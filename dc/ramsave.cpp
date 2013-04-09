#include "compiler.h"
#include "dosio.h"
#include "scrnmng.h"
#include "soundmng.h"
#include "ini.h"
#include "dcsys.h"
#include "ramsave.h"
#include "icon.h"
#include "vmu.h"
#include "ui.h"
#include "sysmenu.h"
#include <zlib.h>
#include <ronin/ta.h>
#include <ronin/dc_time.h>


extern Icon icon;

#ifndef NOSERAL
#define _debug(...) reportf(__VA_ARGS__)
#else
#define _debug(...) do{}while(0)
#endif

void display_progress(double i, double size)
{
	struct polygon_list mypoly;
	struct packed_colour_vertex_list myvertex;
	char str[16];
	float sx, sy;
	const unsigned int bar_w = 320;
	const unsigned int bar_h = 8;
	float x, y;

	tx_resetfont();
  
	scrnmng_update();
	draw_transquad(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, UI_TR, UI_TR, UI_TR, UI_TR);

	x = ui_offx() + 2 * ui_font_width();
	y = ui_offy() + (ui_height() - ui_font_height()+bar_h)/2;

	ui_font_draw(x, y - ui_font_height(), 255, 255, 255, "Loading, do not remove VMU!");
  
	sx = ui_offx() + (ui_width()-bar_w)/2;
	sy = y;

	draw_transquad(sx, sy, sx + bar_w, sy + bar_h, MAKECOL32(64, 64, 64, 128), MAKECOL32(64, 64, 64, 128), MAKECOL32(64, 64, 64, 128), MAKECOL32(64, 64, 64, 128));

	mypoly.cmd =
		TA_CMD_POLYGON|TA_CMD_POLYGON_TYPE_TRANSPARENT|TA_CMD_POLYGON_SUBLIST|
		TA_CMD_POLYGON_STRIPLENGTH_2|TA_CMD_POLYGON_GOURAUD_SHADING;
	mypoly.mode1 = TA_POLYMODE1_Z_ALWAYS|TA_POLYMODE1_NO_Z_UPDATE;
	mypoly.mode2 = TA_POLYMODE2_BLEND_SRC_ALPHA|TA_POLYMODE2_BLEND_DST_INVALPHA|
		TA_POLYMODE2_ENABLE_ALPHA|TA_POLYMODE2_FOG_DISABLED|TA_POLYMODE2_TEXTURE_MODULATE_ALPHA;
	mypoly.texture = 0;
	mypoly.alpha = mypoly.red = mypoly.green = mypoly.blue = 0;
	ta_commit_list(&mypoly);
	myvertex.cmd = TA_CMD_VERTEX;
	myvertex.x = sx;
	myvertex.y = sy;
	myvertex.z = 0.5;
	myvertex.colour = 0xff00e0e0;
	myvertex.ocolour = 0;
	ta_commit_list(&myvertex);

	myvertex.x = sx + bar_w * (1.0/size) * i;
	ta_commit_list(&myvertex);

	myvertex.x = sx;
	myvertex.y = sy + bar_h;
	ta_commit_list(&myvertex);

	myvertex.cmd |= TA_CMD_VERTEX_EOS;
	myvertex.x = sx + bar_w * (1.0/size) * i;
	ta_commit_list(&myvertex);
  
	sprintf(str, "%3.0f", (i/size) * 100);
  
	x = sx + (bar_w - 3 * ui_font_width()) / 2;
	y = sy + bar_h + 2;
	ui_font_draw(x, y, 255, 255, 255, str);
	x += 3 * ui_font_width();
	ui_font_draw(x, y, 255, 255, 255, "%");

	ta_commit_frame();
}


// ----

#define MAX_VM_SIZE (128*1024)

static int checked_vm = 0;

static _CCFILEH fh[DATACACHES];
static int stc_index;

struct Header {
	unsigned int id;
	long pos;
	unsigned int size;
	unsigned int allocsize;
	Header *next;
};

static Header *Head;
static unsigned char _savedata[MAX_VM_SIZE];

struct SaveHeader {
	unsigned int id;
	unsigned int files;
};

struct DataHeader {
	long pos;
	unsigned int size;
};

static void init_save()
{
	ZeroMemory(_savedata, sizeof(_savedata));

	Head = (Header*)_savedata;
	Head->id = 0;
	Head->size = 0;
	Head->allocsize = sizeof(_savedata);
	Head->next = NULL;
}

static void allocSaveData(unsigned int id, long pos, const void *buf, UINT size)
{
	Header *List;
	unsigned int allocsize = size + sizeof(Header);

	for (List=Head; List; List=List->next)
		if (List->id == 0 && List->allocsize >= allocsize)
			break;

	if (List == NULL) {

		_debug("\n%s: out of memory\n", __func__);
		return;
	}
  
	Header *Tmp = (Header*)((char*)List + allocsize);
	Tmp->allocsize = List->allocsize - allocsize;
	Tmp->next = NULL;

	List->allocsize = allocsize;
	List->id = id;
	List->pos = pos;
	List->size = size;
	List->next = Tmp;

	unsigned char *s = (unsigned char *)buf;
	unsigned char *d = (unsigned char *)(List + 1);

	unsigned int n = size;
	while (n--)
		*d++ = *s++;
}

static void pushSaveData(unsigned int id, long pos, const void *buf, UINT size)
{
	Header *List;

	for (List=Head; List; List=List->next) {

		if (id == List->id && pos == List->pos && size <= List->size) {

			unsigned char *s = (unsigned char *)buf;
			unsigned char *d = (unsigned char *)(List + 1);
	
			unsigned int n = size;
			while (n--)
				*d++ = *s++;

			return;
		}
	}

	allocSaveData(id, pos, buf, size);
}

static void loadSaveData(unsigned int id, unsigned char *buf)
{
	SaveHeader *head = (SaveHeader *)buf;
	DataHeader *data = (DataHeader*)((char*)buf + sizeof(SaveHeader));
	unsigned int n = head->files;
  
	while(n--) {
		char *src = ((char*)data + sizeof(DataHeader));
		unsigned int allocsize = data->size + sizeof(DataHeader);
    
		allocSaveData(id, data->pos, src, data->size);

		data = (DataHeader*)((char*)data + allocsize);
	}
}

static int saveSaveData(unsigned int id, unsigned char *buf)
{
	Header *List;
	SaveHeader *head = (SaveHeader *)buf;
	DataHeader *data = (DataHeader*)((char*)buf + sizeof(SaveHeader));
	unsigned int files = 0;
  
	for (List=Head; List; List=List->next) {
		if (id == List->id) {
      
			unsigned int allocsize = List->size + sizeof(DataHeader);
			DataHeader *tmp = (DataHeader*)((char*)data + allocsize);
      
			data->pos = List->pos;
			data->size = List->size;

			unsigned char *s = (unsigned char *)(List + 1);
			unsigned char *d = (unsigned char *)(data + 1);

			unsigned int n = List->size;
			while (n--)
				*d++ = *s++;

			data = tmp;

			++files;
		}
	}

	if (files) {
		head->id = id;
		head->files = files;
	}

	_debug("\n%s (0x%x)\n",__func__, files);

	return files;
}


static void loadSaveDatafromVmu(unsigned int id)
{
	char path[16];
	unsigned char buf[MAX_VM_SIZE];
	int size = MAX_VM_SIZE;
	int res;
	const char *load_title = "Select a VMU to load from";
  
	sprintf(path, "%x", id);
  
	ZeroMemory(buf, sizeof(buf));

	int files = vm_SearchFile(path, &res);

	if (!files) {
		return;
	}

	if (files == 1 && load_from_vmu(res, path, buf, &size)) {

		loadSaveData(id, buf);
    
		checked_vm  = res;
		return;
	}
  
	for (;;) {
    
		if (!vmu_select(&res, load_title))
			return;

		if (!load_from_vmu(res, path, buf, &size)) {

			_debug("Failed!");
		} else {

			loadSaveData(id, buf);

			checked_vm  = res;
			return;
		}
	}
}


static void patchSaveData(unsigned int id, unsigned char *buf)
{
	Header *List;

	for (List=Head; List; List=List->next) {

		if (id == List->id) {

			unsigned char *s = (unsigned char *)(List + 1);
			unsigned char *d = (unsigned char *)(buf + List->pos);

			unsigned int n = List->size;
			while (n--)
				*d++ = *s++;
		}
	}
}

void ccfile_init(void)
{
	CCFILEH cc;

	cc = fh;

	for (int i=0; i<DATACACHES; ++i) {
		cc->name[0] = '\0';
		cc->size = 0;
		cc->id = 0;
		ZeroMemory(cc->ptr, sizeof(cc->ptr));
		cc++;
	}
  
	stc_index = 0;

	init_save();
}

static void SaveData_finalize(unsigned int id)
{
	char path[16];
	char long_desc[32] ;
	char short_desc[16];
	unsigned char buf[MAX_VM_SIZE];
	const char *save_title = "Select a VMU to save";

	if (checked_vm < 0)
		return;

	sprintf(long_desc, "%s/Save File", ini_title);
	milstr_ncpy(short_desc, ini_title ,sizeof(short_desc));


	loadSaveDatafromVmu(id);

  
	ZeroMemory(buf, sizeof(buf));
	sprintf(path, "%x", id);
  
	int files = saveSaveData(id, buf);

	if (!files)
		return;

	int res = checked_vm;

	if (!res && !vmu_select(&res, save_title)) {
		checked_vm = -1;
		return;
	}
  
	for (;;) {
		display_message("Saving, do not power off or remove VMU!");
    
		if (!save_to_vmu(res, path, long_desc, short_desc, buf, MAX_VM_SIZE, &icon)) {

			display_message("Failed!");
			drawlcdstring(res, 0, 8, "FAILED!");

			if (!vmu_select(&res, save_title)) {
				checked_vm = -1;
				return;
			}
		} else {
			display_message("Saved!");
			drawlcdstring(res, 0, 8, "SAVED!");
      
			checked_vm = res;
			return;
		}
	}
}

static int delID(unsigned int id)
{
	Header *List;
	unsigned int files = 0;
  
	for (List=Head; List; List=List->next)
		if (id == List->id) {
			List->id = 0;
			++files;
		}

	_debug("\n%s, %d\n",__func__, files);
	return files;
}

static unsigned int getID()
{
	Header *List;
	unsigned int id = 0;
  
	for (List=Head; List; List=List->next)
		if (List->id != 0) {
			id = List->id;
			break;
		}
  
	return id;
}


void ccfile_term(void)
{
	unsigned int id;

	if (!dc_savedtimes) {
		return;
	}

	soundmng_stop();

	dc_savedtimes = 0;

	for (;;) {
		id = getID();
    
		if (id == 0)
			break;
    
		SaveData_finalize(id);
		delID(id);
	}

	init_save();

	soundmng_play();
}

CCFILEH ccfile_open(const char *fname) {
  
	CCFILEH ret;
	CCFILEH cc;
	CCFILEH stc;
	int fd;

	ret = NULL;
	stc = NULL;
	cc = fh;

	for (int i=0; i<DATACACHES; ++i) {
		if (!milstr_cmp(cc->name, fname)) {
			ret = cc;
			break;
		}
		cc++;
	}

	if (ret == NULL) {

		stc = (CCFILEH)&fh[stc_index++];
		stc_index &= (DATACACHES-1);
	}

	if ((ret == NULL) && (stc != NULL)) {

		fd = open(fname,O_RDONLY);
		if (fd < 0) {
			goto cc_err1;
		}
		stc->size = file_size(fd);

		file_cpyname(stc->name, fname, MAX_PATH);

		if (stc->size > DATACACHESIZE) {
			goto cc_err2;
		}

		soundmng_stop();
	  
		long pos = 0;
		unsigned int size = stc->size;
		unsigned int len = size/256;

    
		ZeroMemory(stc->ptr, DATACACHESIZE);

		int n = 0;
		while (size > 0) {
			int r = pread(fd, stc->ptr + pos, len, pos);
			display_progress(n, 256);
			++n;
			pos += r;
			size -= r;
		}

		soundmng_play();
    
		close(fd);

		uLong adler;
		adler = adler32(0L, Z_NULL, 0);
		stc->id = adler32(adler, stc->ptr, stc->size);

		loadSaveDatafromVmu(stc->id);

		patchSaveData(stc->id, stc->ptr);

		ret = stc;
	}
  
	ret->pos = 0;
  
	return ret;
  
cc_err2:
	close(fd);
  
cc_err1:
	return NULL;
}

CCFILEH ccfile_open_rb(const char *fname) {

	CCFILEH ret;

	ret =  ccfile_open(fname);
	return ret;
}

long ccfile_seek(CCFILEH hdl, long pos, int method) {
  
	long	ret;

	ret = 0;
	if (hdl == NULL) {
		goto cc_exit;
	}

	switch (method) {
	case 1:
		ret = hdl->pos;
		break;
    
	case 2:
		ret = hdl->size;
		break;
	}
	ret += pos;
	if (ret < 0) {
		ret = 0;
	} else if (ret > (int)hdl->size) {
		ret = hdl->size;
	}
  
	hdl->pos = ret;
  
cc_exit:
	return ret;
}

UINT ccfile_read(CCFILEH hdl, void *buf, UINT size) {

	UINT	ret;

	ret = 0;
	if (hdl == NULL) {
		goto cc_exit;
	}
	ret = min(size, hdl->size - hdl->pos);
	if (ret == 0) {
		goto cc_exit;
	}

	CopyMemory(buf, hdl->ptr + hdl->pos, ret);
	hdl->pos += ret;
  
cc_exit:
	return ret;
}

UINT ccfile_write(CCFILEH hdl, const void *buf, UINT size) {

	UINT	ret;

	ret = 0;
	if (hdl == NULL) {
		goto cc_exit;
	}
	ret = min(size, hdl->size - hdl->pos);
	if (ret == 0) {
		goto cc_exit;
	}

	pushSaveData(hdl->id, hdl->pos, buf, ret);

	dc_savedtimes = Timer() + USEC_TO_TIMER(5*1000000);

	CopyMemory(hdl->ptr + hdl->pos, buf, ret);
	hdl->pos += ret;
  
cc_exit:
	return ret;
}

short ccfile_close(CCFILEH hdl) {

	return 0;
}

UINT ccfile_getsize(CCFILEH hdl) {

	UINT	ret;

	ret = 0;
	if (hdl == NULL) {
		goto cc_exit;
	}
	ret = hdl->size;
  
cc_exit:
	return ret;
}


short ccfile_attr(const char *fname) {

	return 0;
}
