#include "compiler.h"
#include "strres.h"
#include "dosio.h"
#include "scrnmng.h"
#include "soundmng.h"
#include "pccore.h"
#include "diskdrv.h"
#include "fddfile.h"
#include "font.h"
#include "sysmenu.h"
#include "filesel.h"
#include "dcsys.h"
#include "event.h"
#include "ui.h"
#include <ronin/ta.h>


static int numfiles = 0;
static int topfile = 0;

struct _flist;
typedef struct _flist	 _FLIST;
typedef struct _flist	 *FLIST;

struct _flist {
	FLIST	next;
	UINT	isdir;
	OEMCHAR	name[MAX_PATH];
};

typedef struct {
	BRESULT		result;
	LISTARRAY	flist;
	FLIST		fbase;
	const OEMCHAR	*filter;
	const OEMCHAR	*ext;
	OEMCHAR		path[MAX_PATH];
} FILESEL;

static	FILESEL		filesel;


// ----

static FLIST getflist(int pos) {

	FLIST	ret;

	ret = NULL;
	if (pos >= 0) {
		ret = filesel.fbase;
		while((pos > 0) && (ret)) {
			pos--;
			ret = ret->next;
		}
	}
	return ret;
}

static BRESULT fappend(LISTARRAY flist, FLINFO *fli) {

	FLIST	fl;
	FLIST	*st;
	FLIST	cur;
  
	fl = (FLIST)listarray_append(flist, NULL);
	if (fl == NULL) {
		return FAILURE;
	}
	fl->isdir = (fli->attr & 0x10)?1:0;
	file_cpyname(fl->name, fli->path, sizeof(fl->name));
	st = &filesel.fbase;
	while (1) {
		cur = *st;
		if (cur == NULL) {
			break;
		}
		if (fl->isdir > cur->isdir) {
			break;
		}
		if ((fl->isdir == cur->isdir) &&
			(file_cmpname(fl->name, cur->name) < 0)) {
			break;
		}
		st = &cur->next;
	}
	fl->next = *st;
	*st = fl;
	return SUCCESS;
}

static BRESULT checkext(const OEMCHAR *path, const OEMCHAR *ext) {

	const OEMCHAR	*p;
  
	if (ext == NULL) {
		return TRUE;
	}
	p = file_getext(path);
	while (*ext) {
		if (!file_cmpname(p, ext)) {
			return TRUE;
		}
		ext += OEMSTRLEN(ext) + 1;
	}
	return FALSE;
}

static void dlgsetlist(const OEMCHAR *ext) {

	LISTARRAY	flist;
	FLISTH		flh;
	FLINFO		fli;
	BRESULT		append;
	FLIST		fl;
  
	filesel.ext = ext;
  
	listarray_destroy(filesel.flist);
	flist = listarray_new(sizeof(_FLIST), 64);
	filesel.flist = flist;
	filesel.fbase = NULL;
	flh = file_list1st(filesel.path, &fli);
	if (flh != FLISTH_INVALID) {
		do {
			append = FALSE;
			if (fli.attr & 0x10) {
				append = TRUE;
			}
			else if (!(fli.attr & 0x08)) {
				append = checkext(fli.path, filesel.ext);
			}
			if (append) {
				if (fappend(flist, &fli) != SUCCESS) {
					break;
				}
			}
		} while (file_listnext(flh, &fli) == SUCCESS);
		file_listclose(flh);
	}
	numfiles = 0;
	fl = filesel.fbase;
	while (fl) {
		fl = fl->next;
		++numfiles;
	}
	//printf("%s numfiles %d\n", __func__, numfiles);
}

static BRESULT dlgupdate(int filepos) {

	FLIST	fl;
  
	fl = getflist(filepos);
	if (fl == NULL) {
		return FALSE;
	}
	file_setseparator(filesel.path, sizeof(filesel.path));
	file_catname(filesel.path, fl->name, sizeof(filesel.path));
	if (fl->isdir) {
		return FALSE;
	}
	else {
		filesel.result = TRUE;
		return TRUE;
	}
}

// ----

static const OEMCHAR fddext[] = OEMTEXT("d88\00088d\0dx1\0002d\0xdf\0hdm\0dup\0002hd\0tfd\0");
static const char fontext[] = "bmp\0rom\0fnt\0x1\0dat\0";

static const char biosext[] = "rom\0x1t\0x1\0";


void filesel_fdd(REG8 drv) {

	if (drv < 4) {
		if (filesel.result) {
			//printf("%s %s\n", __func__, filesel.path);
      
			diskdrv_setfdd(drv, filesel.path, 0);
		}
	}
}

// ---

#define MAX_FILE 12

struct File {
	char string[MAX_PATH];
	float x, y;
};

static int sel;

static int init_file(File *di, int max_file, int type)
{
	FLIST fl;
	OEMCHAR work[MAX_PATH];

	int n;
	float x,y;
  
	x = 0;
	y = 0;
	n = 0;

	if (!topfile) {
    
		switch(type) {
      
		case HDD_FILE:
			break;

		case BIOS_FILE:
			dlgsetlist(biosext);
			break;
      
		case FONT_FILE:
			dlgsetlist(fontext);
			break;
      
		default:
		case FDD_FILE:
			dlgsetlist(fddext);
			break;
		}
	}

	fl = getflist(topfile);
          
	if (fl) {
		for (int i = 0; i < max_file; ++i) {
      
			if (!fl)
				break;
	
			if (fl->isdir) {
				milstr_ncpy(work, fl->name, NELEMENTS(work));
				milstr_ncat(work, "\\", NELEMENTS(work));
			} else {
				milstr_ncpy(work, fl->name, NELEMENTS(work));
			}
			di[n].x = x;
			di[n].y = y;

			memcpy(di[i].string ,work, NELEMENTS(work));
			y += ui_font_height() + 5;
			++n;

			fl = fl->next;
		}
	} else {
		file_cutname(filesel.path);
		file_cutseparator(filesel.path);
	}
  
	return n;
}

static int select_file(int drv, File *di, int num_items, int type)
{
	REG8 r;
	OEMCHAR work[32];
	int filepos;
	float x,y;
	int cur_file;
	int max_file;

	if (!num_items) {
		return -1;
	}

	float base_width = ui_width();
	float base_height = MAX_FILE * (ui_font_height() + 5) + (ui_font_height() + 10);

	float offx = ui_offx() + (ui_width() - base_width) / 2;
	float offy = ui_offy() + (ui_height() - base_height) / 2;

	cur_file = topfile/MAX_FILE;
	max_file = numfiles/MAX_FILE;

	if(numfiles%MAX_FILE)
		++max_file;
	
	switch(type) {
    
	case HDD_FILE:
		break;
    
	case FONT_FILE:
		OEMSPRINTF(work, "Select Font p.%d/%d", cur_file+1, max_file);
		break;

	case BIOS_FILE:
		OEMSPRINTF(work, "Select BIOS p.%d/%d", cur_file+1, max_file);
		break;
    
	default:
	case FDD_FILE:
		OEMSPRINTF(work, "Select FDD%d p.%d/%d", drv, cur_file+1, max_file);
	}

	if (sel < 0)
		sel = 0;
	else if (sel > (num_items - 1))
		sel = num_items - 1;

	int fade = 128;
  
	for (;;) {

		uisys_task();
      
		if (ui_keyrepeat(JOY_UP)) {
			if (--sel < 0)
				sel = num_items - 1;
		} else if (ui_keyrepeat(JOY_DOWN)) {
			if (++sel > (num_items - 1))
				sel = 0;
		} else if (ui_keyrepeat(JOY_RIGHT) || ui_keyrepeat(JOY_RTRIGGER)) {
			if ((topfile + MAX_FILE) < numfiles) {
	
				if ((topfile += MAX_FILE) > (numfiles - 1))
					topfile = numfiles - 1;

				return 0;
			}
		} else if (ui_keyrepeat(JOY_LEFT) || ui_keyrepeat(JOY_LTRIGGER)) {
			if ((topfile - MAX_FILE) >= 0) {
	
				if ((topfile -= MAX_FILE) < 0)
					topfile = 0;

				return 0;
			}
		}

		if (ui_keypressed(JOY_A)) {
      
			filepos = topfile + sel;
      
			if (dlgupdate(filepos)) {

				switch (type) {
	  
				case HDD_FILE:
					/* NULL */
					break;
	  
				case FONT_FILE:
					r = font_load(filesel.path, TRUE);
					break;

				case BIOS_FILE:
					/* NULL */
					break;
	  
				default:
				case FDD_FILE:
					filesel_fdd(drv);
				}

				file_cutname(filesel.path);
				file_cutseparator(filesel.path);

				return -2;
			} else { //is_dir
				topfile = 0;
				sel = 0;

				return 0;
			}
		}
    
		if (ui_keypressed(JOY_Y)) {
    
			if (filesel.path) {
				file_cutname(filesel.path);
				file_cutseparator(filesel.path);
	
				topfile = 0;
				sel = 0;
			}
      
			return 0;
		}
    
		if (ui_keypressed(JOY_B))
			return -1;

		if ((fade += 255/60) >= 255) fade = 128;
    
		tx_resetfont();
    
		scrnmng_update();
		draw_transquad(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, UI_TR, UI_TR, UI_TR, UI_TR);
		draw_transquad(offx, offy, offx + base_width, offy + base_height, UI_BS, UI_BS, UI_BS, UI_BS);
    
		x = offx + (base_width - strlen(work) * ui_font_width()) / 2;
		y = offy;
		ui_font_draw(x, y, 255, 255, 255, work);
    
		if (cur_file != 0)
			draw_pointer(offx + ui_font_width(), y, 0);
    
		if (cur_file != (max_file - 1))
			draw_pointer(offx + base_width - ui_font_width(), y, 1);
    
		x = offx;
		y += ui_font_height() + 5;

		draw_transquad(offx, y, offx + base_width, y + 1, MAKECOL32(128, 128, 128, 255),
					   MAKECOL32(128, 128, 128, 255), MAKECOL32(128, 128, 128, 255), MAKECOL32(128, 128, 128, 255));
    
		for (int i = 0; i < num_items; ++i) {
      
			if (i == sel)

				draw_transquad(x + di[i].x, y + di[i].y, offx + base_width, y + di[i].y +  ui_font_height() + 5, MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade));

			ui_font_draw(x + di[i].x, y + di[i].y, 255, 255, 255, di[i].string);
		}
    
		x = ui_offx();
		y = ui_height() - ui_font_height();
    
		ui_font_draw(x, y, 255, 255, 255, "A:Select B:Back Y:Go up");
    
		ta_commit_frame();
	}
}

void file_browser(int drv, int type)
{
	File files[MAX_FILE];
	static int last = -1;
	int n;

	if (last != type) {
		topfile = 0;
		sel = 0;
		milstr_ncpy(filesel.path, "XMIL", NELEMENTS(filesel.path));
    
		last = type;
	}
  
	for (;;) {
    
		n = init_file(files, MAX_FILE, type);
    
		if (select_file(drv, files, n, type) < 0)
			break;
	}
}
