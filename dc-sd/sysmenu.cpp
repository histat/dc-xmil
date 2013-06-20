#include "compiler.h"
#include "strres.h"
#include "xmil.h"
#include "fontmng.h"
#include "scrnmng.h"
#include "sysmng.h"
#include "taskmng.h"
#include "dckbd.h"
#include "z80core.h"
#include "pccore.h"
#include "iocore.h"
#include "keystat.h"
#include "scrnsave.h"
#include "soundmng.h"
#include "diskdrv.h"
#include "sysmenu.h"
#include "sysmenu.res"
#include "sysmenu.str"
#include "filesel.h"
#include "dlgabout.h"
#include "dlgcfg.h"
#include "dcsys.h"
#include "event.h"
#include "ui.h"
#include <time.h>
#include <ronin/ta.h>


static void bmpsave()
{
	SCRNSAVE hdl;
	char fname[32];
	time_t long_time;
	struct tm *now_time;

	display_message("Saving...");
	
	hdl = scrnsave_get();

    time(&long_time);
    now_time = localtime(&long_time);

    OEMSPRINTF(fname, "%d%d%d%d%d%d.BMP"
	       ,now_time->tm_year + 1900
	       ,now_time->tm_mon + 1
	       ,now_time->tm_mday
	       ,now_time->tm_hour
	       ,now_time->tm_min
	       ,now_time->tm_sec);

	scrnsave_writebmp(hdl, fname, SCRNSAVE_8BIT);
	//scrnsave_writegif(hdl, fname, SCRNSAVE_AUTO);
	
	scrnsave_trash(hdl);
}

static void sys_cmd(MENUID id) {

	UINT	update;

	update = 0;
	switch(id) {
	case MID_IPLRESET:
		pccore_reset();
		break;

	case MID_NMIRESET:
		Z80_NMI();
		break;

	case MID_CONFIG:
		dlgcfg();
		break;

	case MID_CHANGEFONT:
		file_browser(0xff, FONT_FILE);
		break;
			
	case MID_FDD0OPEN:
		file_browser(0, FDD_FILE);
		break;

	case MID_FDD0EJECT:
		diskdrv_setfdd(0, NULL, 0);
		break;

	case MID_FDD1OPEN:
		file_browser(1, FDD_FILE);
		break;

	case MID_FDD1EJECT:
		diskdrv_setfdd(1, NULL, 0);
		break;

	case MID_X1ROM:
		xmilcfg.ROM_TYPE = 1;
		update = SYS_UPDATECFG;
		break;

	case MID_TURBO:
		xmilcfg.ROM_TYPE = 2;
		update = SYS_UPDATECFG;
		break;

#if defined(SUPPORT_TURBOZ)
	case MID_TURBOZ:
		xmilcfg.ROM_TYPE = 3;
		update = SYS_UPDATECFG;
		break;
#endif

	case MID_BOOT2D:
		xmilcfg.DIP_SW &= ~DIPSW_BOOTMEDIA;
		update = SYS_UPDATECFG;
		break;

	case MID_BOOT2HD:
		xmilcfg.DIP_SW |= DIPSW_BOOTMEDIA;
		update = SYS_UPDATECFG;
		break;

	case MID_HIGHRES:
		xmilcfg.DIP_SW &= ~DIPSW_RESOLUTE;
		update = SYS_UPDATECFG;
		break;

	case MID_LOWRES:
		xmilcfg.DIP_SW |= DIPSW_RESOLUTE;
		update = SYS_UPDATECFG;
		break;

	case MID_DISPSYNC:
		xmilcfg.DISPSYNC ^= 1;
		update |= SYS_UPDATECFG;
		break;

	case MID_RASTER:
		xmilcfg.RASTER ^= 1;
		update |= SYS_UPDATECFG;
		break;

	case MID_NOWAIT:
		xmiloscfg.NOWAIT ^= 1;
		update |= SYS_UPDATECFG;
		break;

	case MID_AUTOFPS:
		xmiloscfg.DRAW_SKIP = 0;
		update |= SYS_UPDATECFG;
		break;

	case MID_60FPS:
		xmiloscfg.DRAW_SKIP = 1;
		update |= SYS_UPDATECFG;
		break;

	case MID_30FPS:
		xmiloscfg.DRAW_SKIP = 2;
		update |= SYS_UPDATECFG;
		break;

	case MID_20FPS:
		xmiloscfg.DRAW_SKIP = 3;
		update |= SYS_UPDATECFG;
		break;

	case MID_15FPS:
		xmiloscfg.DRAW_SKIP = 4;
		update |= SYS_UPDATECFG;
		break;

	case MID_CURDEF:
		dckbd_bindcur(0);
		xmiloscfg.bindcur = 0;
		update |= SYS_UPDATEOSCFG;
		break;

	case MID_CUR1:
		dckbd_bindcur(1);
		xmiloscfg.bindcur = 1;
		update |= SYS_UPDATEOSCFG;
		break;

	case MID_CUR2:
		dckbd_bindcur(2);
		xmiloscfg.bindcur = 2;
		update |= SYS_UPDATEOSCFG;
		break;

	case MID_BTNDEF:
		dckbd_bindbtn(0);
		xmiloscfg.bindbtn = 0;
		update |= SYS_UPDATEOSCFG;
		break;

	case MID_BTN1:
		dckbd_bindbtn(1);
		xmiloscfg.bindbtn = 1;
		update |= SYS_UPDATEOSCFG;
		break;

	case MID_BTN2:
		dckbd_bindbtn(2);
		xmiloscfg.bindbtn = 2;
		update |= SYS_UPDATEOSCFG;
		break;

	case MID_KEY:
		xmilcfg.KEY_MODE = 0;
		keystat_resetjoykey();
		update |= SYS_UPDATECFG;
		break;

	case MID_JOY1:
		xmilcfg.KEY_MODE = 1;
		keystat_resetjoykey();
		update |= SYS_UPDATECFG;
		break;

	case MID_JOY2:
		xmilcfg.KEY_MODE = 2;
		keystat_resetjoykey();
		update |= SYS_UPDATECFG;
		break;

#if 0
	case MID_MOUSEKEY:
		xmilcfg.KEY_MODE = 3;
		keystat_resetjoykey();
		update |= SYS_UPDATECFG;
		break;
#endif

	case MID_FMBOARD:
		xmilcfg.SOUND_SW ^= 1;
		update = SYS_UPDATECFG;
		break;

	case MID_BMPSAVE:
		bmpsave();
		break;
			
	case MID_SEEKSND:
		xmilcfg.MOTOR ^= 1;
		update |= SYS_UPDATECFG;
		break;

	case MID_JOYX:
		xmilcfg.BTN_MODE ^= 1;
		update |= SYS_UPDATECFG;
		break;

	case MID_RAPID:
		xmilcfg.BTN_RAPID ^= 1;
		update |= SYS_UPDATECFG;
		break;

	case MID_ABOUT:
		dlgabout();
		break;

	case MID_EXIT:
		taskmng_exit();
		break;
	}
	sysmng_update(update);
}

// ---

struct Menu {
	const OEMCHAR	*string;
	const MSYSITEM *child;
	MENUID	id;
	float x,y;
	int r, g, b;
};

int create_menu(Menu *dst, const MSYSITEM *src)
{
	int i;
	int n;
	float x,y;

	n = 0;
	x = 0;
	y = 0;
  
	i = 0;
	for (;;) {
    
		if (src[i].string != NULL) {
      
			int len = strlen(src[i].string);
			x = (MAX_MENU_SIZE - len * ui_font_width()) / 2;
      
			dst[n].id = src[i].id;
			dst[n].string = src[i].string;
			dst[n].child = src[i].child;
			dst[n].x = x;
			dst[n].y = y;

			if (src[i].flag & MENU_GRAY) {
				dst[n].r = 0xa9;
				dst[n].g = 0xa9;
				dst[n].b = 0xa9;
			} else {
				dst[n].r = 255;
				dst[n].g = 255;
				dst[n].b = 255;
			}

			y += ui_font_height() + 5;

			++n;
		}

		if (src[i].flag & MENU_SEPARATOR)
			y += ui_font_height() / 2;
    
		if (src[i].flag & MENU_DELETED)
			break;

		++i;
	}
  
	return n;
}

void setcheck(MENUID arg, float x, float y, MENUID id, int checked)
{
	if (arg == id && checked) {
		draw_pointer(x - ui_font_width(), y, 1);
	}
}

#define MAX_LINK 5
struct Link {
	const MSYSITEM *cur_menu;
	int sel;
};

static const MSYSITEM *cur_menu;
static int menu_index;
static Link link_menu[MAX_LINK];

static int main_menu(Menu *menu, int num_items)
{
	int i;
	float x,y;
	float base_width = MAX_MENU_SIZE;
	float base_height = 0;

	UINT8	b;
	MENUID id;
  
	base_height += (menu[1].y - menu[0].y);
	for(i=0; i<num_items-1; ++i) {
		base_height += (menu[i+1].y - menu[i].y);
	}
  
	int sel = link_menu[menu_index].sel;

	float offx = ui_offx() + (ui_width() - base_width) / 2;
	float offy = ui_offy() + (ui_height() - base_height) / 2;

	int fade = 128;

	for (;;) {
		uisys_task();
      
		if (ui_keyrepeat(JOY_UP)) {
			if (--sel < 0)
				sel = num_items - 1;
		} else if (ui_keyrepeat(JOY_DOWN)) {
			if (++sel > (num_items - 1))
				sel = 0;
		}
    
		if (ui_keypressed(JOY_A)) {

			link_menu[menu_index].sel = sel;
			
			if (menu[sel].child) {
				++menu_index;
				link_menu[menu_index].cur_menu = menu[sel].child;
				link_menu[menu_index].sel = 0;
				return 0;
			}

			id = menu[sel].id;
			sys_cmd(id);
      
			switch(id) {
			case MID_EXIT:
			case MID_IPLRESET:
			case MID_NMIRESET:
				return -1;

			case MID_CONFIG:
			case MID_ABOUT:
				return 0;
			}
		}
    
		if (ui_keypressed(JOY_B)) {
      
			if (--menu_index < 0)
				menu_index = 0;

			if (cur_menu != s_main)
				return 0;
		}

		if (ui_keypressed(JOY_START))
			return -1;


		if ((fade += 255/60) > 255) fade = 128;
      
		tx_resetfont();
      
		scrnmng_update();
		draw_transquad(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, UI_TR, UI_TR, UI_TR, UI_TR);
		draw_transquad(offx, offy, offx + base_width, offy + base_height, UI_BS, UI_BS, UI_BS, UI_BS);
    
		for (i = 0; i < num_items; ++i) {
      
			y = offy + menu[i].y;
	
			if (i == sel) {
				draw_transquad(offx, y, offx + base_width, y + ui_font_height() + 5, MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade));	
			}
      
			x = offx + menu[i].x;
			ui_font_draw(x, y, menu[i].r, menu[i].g, menu[i].b, menu[i].string);
      
			b = xmilcfg.ROM_TYPE;
			setcheck(menu[i].id, x, y, MID_X1ROM, (b == 1));
			setcheck(menu[i].id, x, y, MID_TURBO, (b == 2));
			setcheck(menu[i].id, x, y, MID_TURBOZ, (b == 3));
			b = xmilcfg.DIP_SW & DIPSW_BOOTMEDIA;
			setcheck(menu[i].id, x, y, MID_BOOT2D, (b == 0));
			setcheck(menu[i].id, x, y, MID_BOOT2HD, (b != 0));
			b = xmilcfg.DIP_SW & DIPSW_RESOLUTE;
			setcheck(menu[i].id, x, y, MID_HIGHRES, (b == 0));
			setcheck(menu[i].id, x, y, MID_LOWRES, (b != 0));
			setcheck(menu[i].id, x, y, MID_DISPSYNC, (xmilcfg.DISPSYNC & 1));
			setcheck(menu[i].id, x, y, MID_RASTER, (xmilcfg.RASTER & 1));
			setcheck(menu[i].id, x, y, MID_NOWAIT, (xmiloscfg.NOWAIT & 1));
			b = xmiloscfg.DRAW_SKIP;
			setcheck(menu[i].id, x, y, MID_AUTOFPS, (b == 0));
			setcheck(menu[i].id, x, y, MID_60FPS, (b == 1));
			setcheck(menu[i].id, x, y, MID_30FPS, (b == 2));
			setcheck(menu[i].id, x, y, MID_20FPS, (b == 3));
			setcheck(menu[i].id, x, y, MID_15FPS, (b == 4));
			b = xmiloscfg.bindcur;
			setcheck(menu[i].id, x, y, MID_CURDEF, (b == 0));
			setcheck(menu[i].id, x, y, MID_CUR1, (b == 1));
			setcheck(menu[i].id, x, y, MID_CUR2, (b == 2));
			b = xmiloscfg.bindbtn;
			setcheck(menu[i].id, x, y, MID_BTNDEF, (b == 0));
			setcheck(menu[i].id, x, y, MID_BTN1, (b == 1));
			setcheck(menu[i].id, x, y, MID_BTN2, (b == 2));
			b = xmilcfg.KEY_MODE;
			setcheck(menu[i].id, x, y, MID_KEY, (b == 0));
			setcheck(menu[i].id, x, y, MID_JOY1, (b == 1));
			setcheck(menu[i].id, x, y, MID_JOY2, (b == 2));
			setcheck(menu[i].id, x, y, MID_MOUSEKEY, (b == 3));
			setcheck(menu[i].id, x, y, MID_FMBOARD, (xmilcfg.SOUND_SW & 1));
			setcheck(menu[i].id, x, y, MID_SEEKSND, (xmilcfg.MOTOR & 1));
			setcheck(menu[i].id, x, y, MID_JOYX, (xmilcfg.BTN_MODE & 1));
			setcheck(menu[i].id, x, y, MID_RAPID, (xmilcfg.BTN_RAPID & 1));
		}
      
		x = ui_offx();
		y = ui_height() - ui_font_height();
    
		if (!menu_index) {
			ui_font_draw(x, y, 255, 255, 255, "A:Select");
		} else {
			ui_font_draw(x, y, 255, 255, 255, "A:Select B:Back");
		}
    
		ta_commit_frame();
	}
}

void sysmenu_menuopen()
{
	Menu mainmenu[16];
	int n;

	soundmng_stop();

	menu_index = 0;
	link_menu[menu_index].cur_menu = s_main;
	link_menu[menu_index].sel = 0;

	for (;;) {
		cur_menu = link_menu[menu_index].cur_menu;
		n = create_menu(mainmenu, cur_menu);

		if (main_menu(mainmenu, n) < 0)
			break;
	}
  
	soundmng_play();
}
