#include "compiler.h"
#include "ini.h"
#include "scrnmng.h"
#include <time.h>
#include <zlib.h>
#include "dcsys.h"
#include "event.h"
#include "icon.h"
#include "vmu.h"
#include "ui.h"
#include <ronin/ta.h>
#include <ronin/maple.h>
#include <ronin/dc_time.h>
#include <ronin/vmsfs.h>
#include "vmicon.h"
#include "m4x8.h"

extern Icon icon;

static void setlcd(struct vmsinfo *info, void *bit)
{
	unsigned int param[50];

	param[0] = MAPLE_FUNC_LCD<<24;
	param[1] = 0;
	memcpy(param+2, bit, 48*4);
	maple_docmd(info->port, info->dev, MAPLE_COMMAND_BWRITE, 50, param);
}

static void clearlcd(struct vmsinfo *info)
{
	unsigned int bit[50];
  
	memset(bit, 0, sizeof(bit));
	setlcd(info, bit);
}

static void lcd_putc(unsigned char *bit, int x, int y, int ch)
{
	if((ch < 0x20) || (ch > 0x7a)) return;
  
	const unsigned char *src = m4x8 + 8 * (ch - 0x20);
	unsigned char *dst = bit + (48/8) * 32;
  
	dst -= y * 6 + x;

	for(int i=0; i<8; ++i) {
		int b = *src++;
		unsigned char v = 0;
		for(int j=0; j<8; ++j) {
			v <<= 1;
			v |= (b & 1)?1:0;
			b >>= 1;
		}
		*--dst = v;
		dst -= 5;
	}
}

void drawlcdstring(int vm, int x, int y, const char *str)
{
	int ch;
	unsigned char bit[(48/8)*32];
	struct vmsinfo info;
	struct superblock super;
  
	memset(bit, 0, sizeof(bit));

	if (x < 0 || x > 10)
		x = 0;
	if (y < 0 || y > 31)
		y = 0;
  
	while (ch = *str++) {
		lcd_putc(bit, x, y, ch);
		++x;
		if (x > 10) {
			y += 8;
			x = 0;
		}
	}

	vmsfs_check_unit(vm, 0, &info);
	vmsfs_get_superblock(&info, &super);
	setlcd(&info, bit);
  
	usleep(1000000);
	clearlcd(&info);
}

int vm_SearchFile(const char *fname, int *vm)
{
	struct vmsinfo info;
	struct superblock super;
	struct vms_file file;
	int num_files = 0;
	int tmp = 0;
  
	for (int i=0; i<24; ++i)
		if (vmsfs_check_unit(i, 0, &info))
			if (vmsfs_get_superblock(&info, &super))
				if (vmsfs_open_file(&super, fname, &file)) {
					tmp = i;
					++num_files;
				}
  
	*vm = tmp;
  
	return num_files;
}

// ----


static  void *icon_texture;

void icon_create_texture()
{
	icon_texture = tx_getwork(32*32*2);
	unsigned short *dst = (unsigned short *)icon_texture;
  
	for (int y=0; y<32; ++y) {
		for (int x=0; x<16; ++x) {
			int pix  = vmicon_data[y*16+x];
			*dst++ = vmicon_pal[(pix>>4)&0x0f]; 
			*dst++ = vmicon_pal[(pix>>0)&0x0f];
		}
	}
}

void icon_draw(float x, float y)
{
	struct polygon_list mypoly;
	struct packed_colour_vertex_list myvertex;

	mypoly.cmd =
		TA_CMD_POLYGON|TA_CMD_POLYGON_TYPE_TRANSPARENT|TA_CMD_POLYGON_SUBLIST|
		TA_CMD_POLYGON_STRIPLENGTH_2|TA_CMD_POLYGON_TEXTURED|TA_CMD_POLYGON_PACKED_COLOUR;
	mypoly.mode1 = TA_POLYMODE1_Z_ALWAYS|TA_POLYMODE1_NO_Z_UPDATE;
	mypoly.mode2 = TA_POLYMODE2_BLEND_SRC_ALPHA|TA_POLYMODE2_BLEND_DST_INVALPHA|
		TA_POLYMODE2_ENABLE_ALPHA|TA_POLYMODE2_FOG_DISABLED|
		TA_POLYMODE2_U_SIZE_32|TA_POLYMODE2_V_SIZE_32;
	mypoly.texture = TA_TEXTUREMODE_ARGB4444|TA_TEXTUREMODE_NON_TWIDDLED
		|TA_TEXTUREMODE_ADDRESS(icon_texture);
	mypoly.alpha = mypoly.red = mypoly.green = mypoly.blue = 0;
	ta_commit_list(&mypoly);
  
	myvertex.colour = 0;
	myvertex.ocolour = 0;
	myvertex.z = 0.5;
	myvertex.x = x;
	myvertex.y = y;
	myvertex.cmd = TA_CMD_VERTEX;
	myvertex.u = 0.0;
	myvertex.v = 0.0;
	ta_commit_list(&myvertex);
  
	myvertex.u = 1.0;
	myvertex.x = x+32;
	ta_commit_list(&myvertex);
  
	myvertex.u = 0.0;
	myvertex.v = 1.0;
	myvertex.x = x;
	myvertex.y = y+32;
	ta_commit_list(&myvertex);
  
	myvertex.u = 1.0;
	myvertex.x = x+32;
	myvertex.cmd |= TA_CMD_VERTEX_EOS;
	ta_commit_list(&myvertex);
}

bool vmu_select(int *vm, const char *desc)
{
#define MAX_VMU (4*2)
	bool vmu_avail[MAX_VMU];
	const char *exit_mess = "Press START Button to exit";
	char buf[16];

	tx_resetwork();
	icon_create_texture();
  
	int xpos = 0;
	int ypos = 0;

	for (;;) {

		uisys_task();

		if (ui_keyrepeat(JOY_LEFT)) {
			if (--xpos < 0)
				xpos = 0;
		} else if (ui_keyrepeat(JOY_RIGHT)) {
			if (++xpos > (4-1))
				xpos = (4-1);
		}
		if (ui_keyrepeat(JOY_UP)) {
			if (--ypos < 0)
				ypos = 0;
		} else if (ui_keyrepeat(JOY_DOWN)) {
			if (++ypos > (2-1))
				ypos = (2-1);
		}
    
		if (ui_keypressed(JOY_START)) {

			scrnmng_update();
			commit_dummy_transpoly();
			ta_commit_frame();

			return false;
		}
    
		if (ui_keypressed(JOY_A)) {

			int res = xpos + ypos*6 + 1;
      
			if (vmu_avail[xpos+ypos*4]) {
	
				*vm = res;

				scrnmng_update();
				commit_dummy_transpoly();
				ta_commit_frame();

				return true;
			}
		}

		int i;
		float x,y;
    
		memset(vmu_avail, false, sizeof(vmu_avail));
      
		int mask = getimask();
		setimask(15);
		struct mapledev *pad = locked_get_pads();
      
		for (i=0; i<MAX_VMU; ++i) {
			if (pad[i&3].present & (1<<(i>>2))) {
				vmu_avail[i] = true;
			}
		}
    
		setimask(mask);
      
		tx_resetfont();

		scrnmng_update();

		draw_transquad(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xcc000000, 0xcc000000, 0xcc000000, 0xcc000000);
      
		x = ui_offx() + (ui_width() - strlen(desc) * ui_font_width()) / 2;
		y = ui_offy() + ui_font_height() * 2;
    
		ui_font_draw(x, y, 255, 255, 255, desc);

		float offx = 100;
		float offy = y + ui_font_height() * 4;
      
		x = offx + ui_font_width() * 8;
		y = offy;

    
		for (i=0; i<MAX_VMU; ++i) {
      
			if (i == MAX_VMU/2) {
				x = offx + ui_font_width() * 8;
				y += (64 + 5);
			}

			if (i == (xpos + ypos * 4)) {
				draw_transquad(x, y, x+64, y+64, 0x8000ffff, 0x8000ffff, 0x8000ffff, 0x8000ffff);
			} else {
				draw_transquad(x, y, x+64, y+64, 0xff00ffff, 0xff00ffff, 0xff00ffff, 0xff00ffff);
			}
	
			if (vmu_avail[i]) {
				icon_draw(x + (64 - 32) / 2, y + (64 - 32) / 2);
			}
      
			if (i == (xpos + ypos * 4) && vmu_avail[i]) {
	
				sprintf(buf, "Port %c" ,'A' + xpos);
				ui_font_draw(offx, offy, 255, 255, 0, buf);
	
				sprintf(buf, "Slot %d" ,ypos + 1);
				ui_font_draw(offx, offy+ui_font_height() + 5, 255, 255, 0, buf);
			}
	
			x += (64+5);
		}

		x = ui_offx() + (ui_width() - strlen(exit_mess) * ui_font_width()) / 2;
		y = ui_height() - ui_font_height() * 2;
    
		ui_font_draw(x, y, 255, 255, 255, exit_mess);
    
		ta_commit_frame();
	}
}


#define MAX_VMU_SIZE (128 * 1024)

bool save_to_vmu(int unit, const char *filename, const char *desc_long, const char *desc_short, void *buf, int buf_len, Icon *icon)
{
	struct vms_file_header header;
	struct vmsinfo info;
	struct superblock super;
	struct vms_file file;
	char new_filename[16];
	int free_cnt;
	time_t long_time;
	struct tm *now_time;
	struct timestamp stamp;
	unsigned char icon_buff[32+512];
	unsigned char compressed_buf[MAX_VMU_SIZE];
	int compressed_len;

	memset(compressed_buf, 0, sizeof(compressed_buf));
	compressed_len = buf_len + 512;

	if (compress((Bytef*)compressed_buf, (uLongf*)&compressed_len,
				 (Bytef*)buf, buf_len) != Z_OK) {
		return false;
	}
	if (!vmsfs_check_unit(unit, 0, &info)) {
		return false;
	}
	if (!vmsfs_get_superblock(&info, &super)) {
		return false;
	}
	free_cnt = vmsfs_count_free(&super);

	strncpy(new_filename, filename, sizeof(new_filename));
	if (vmsfs_open_file(&super, new_filename, &file))
		free_cnt += file.blks;

	if (((128+512+compressed_len+511)/512) > free_cnt) {
		return false;
	}

	icon->getIcon(icon_buff);
	memset(&header, 0, sizeof(header));
	strncpy(header.shortdesc, desc_short,sizeof(header.shortdesc));
	strncpy(header.longdesc, desc_long , sizeof(header.longdesc));
	strncpy(header.id, ini_title, sizeof(header.id));
	memcpy(header.palette, icon_buff, sizeof(header.palette));
	header.numicons = 1;

	time(&long_time);
	now_time = localtime(&long_time);
	if (now_time != NULL) {
		stamp.year = now_time->tm_year + 1900;
		stamp.month = now_time->tm_mon + 1;
		stamp.wkday = now_time->tm_wday;
		stamp.day = now_time->tm_mday;
		stamp.hour = now_time->tm_hour;
		stamp.minute = now_time->tm_min;
		stamp.second = now_time->tm_sec;
	}

	if (!vmsfs_create_file(&super, new_filename, &header, icon_buff+sizeof(header.palette), NULL, compressed_buf, compressed_len, &stamp)) {
#ifndef NOSERIAL
		fprintf(stderr,"%s",vmsfs_describe_error());
#endif
		return false;
	}
  
	return true;
}

bool load_from_vmu(int unit, const char *filename, void *buf, int *buf_len)
{
	struct vmsinfo info;
	struct superblock super;
	struct vms_file file;
	char new_filename[16];
	unsigned char compressed_buf[MAX_VMU_SIZE];
	unsigned int compressed_len;

	strncpy(new_filename, filename, sizeof(new_filename));
  
	if (!vmsfs_check_unit(unit, 0, &info)) {
		return false;
	}
	if (!vmsfs_get_superblock(&info, &super)) {
		return false;
	}
	if (!vmsfs_open_file(&super, new_filename, &file)) {
		return false;
	}

	memset(compressed_buf, 0, sizeof(compressed_buf));
	compressed_len = file.size;
  
	if (!vmsfs_read_file(&file, (Bytef*)compressed_buf, compressed_len)) {
		return false;
	}

	if (uncompress((Bytef*)buf, (uLongf*)buf_len,(const Bytef*)compressed_buf, (uLong)compressed_len) != Z_OK) {
		return false;
	}
  
	return true;
}
