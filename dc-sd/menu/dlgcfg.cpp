#include "compiler.h"
#include "strres.h"
#include "xmil.h"
#include "sysmng.h"
#include "pccore.h"
#include "scrnmng.h"
#include "sysmenu.h"
#include "sysmenu.res"
#include "palettes.h"
#include "dlgcfg.h"
#include "dcsys.h"
#include "event.h"
#include "ui.h"
#include <ronin/ta.h>


enum {
	DID_Z80A = 0,
	DID_Z80B,
	DID_Z80H,
	DID_RATE11,
	DID_RATE22,
	DID_RATE44,
	DID_BUFFER,
	DID_BUFSTR,
	DID_SKIPLINE,
	DID_SKIPRATIO,
	DID_SKIPLIGHT,
	DID_SKIPSTR
};

static const OEMCHAR mstr_cfg[] = OEMTEXT("Configure");

static const OEMCHAR str_cpu[] = OEMTEXT("CPU");
static const OEMCHAR str_z80a[] = OEMTEXT("Z80A - 4MHz");
static const OEMCHAR str_z80b[] = OEMTEXT("Z80B - 6MHz");
static const OEMCHAR str_z80h[] = OEMTEXT("Z80H - 8MHz");
static const OEMCHAR str_sound[] = OEMTEXT("Sound");
static const OEMCHAR str_rate[] = OEMTEXT("Rate");
static const OEMCHAR str_11khz[] = OEMTEXT("11KHz");
static const OEMCHAR str_22khz[] = OEMTEXT("22KHz");
static const OEMCHAR str_44khz[] = OEMTEXT("44KHz");
static const OEMCHAR str_buffer[] = OEMTEXT("Buffer");

static const OEMCHAR str_screen[] = OEMTEXT("Screen");
static const OEMCHAR str_skipline[] = OEMTEXT("Use skipline revisions");
static const OEMCHAR str_ratio[] = OEMTEXT("Ratio");

static const MENUPRM res_cfg[] = {
	{DLGTYPE_FRAME,	DID_STATIC,	0,	str_cpu},
	{DLGTYPE_RADIO,	DID_Z80A,	0,	str_z80a},
	{DLGTYPE_RADIO,	DID_Z80B,	MENU_GRAY, str_z80b},
	{DLGTYPE_RADIO,	DID_Z80H,	MENU_GRAY, str_z80h},
	{DLGTYPE_FRAME,	DID_STATIC,	0,	str_sound},
	{DLGTYPE_LTEXT,	DID_STATIC,	0,	str_rate},
	{DLGTYPE_RADIO,	DID_RATE11,	0,	str_11khz},
	{DLGTYPE_RADIO,	DID_RATE22,	0,	str_22khz},
	{DLGTYPE_RADIO,	DID_RATE44,	0,	str_44khz},
	{DLGTYPE_FRAME,	DID_STATIC,	0,	str_screen},
	{DLGTYPE_CHECK,	DID_SKIPLINE,	0,	str_skipline},
	{DLGTYPE_LTEXT,	DID_SKIPRATIO,	0,	str_ratio},
	{DLGTYPE_SLIDER,	DID_SKIPLIGHT,	0, (void *)SLIDERPOS(0, 255)},
	{DLGTYPE_RTEXT,	DID_SKIPSTR,	0, NULL}};


// ----

static int dlg_val[DID_SKIPLIGHT+1];

static int menudlg_getval(MENUID id)
{
	switch (id) {
    
	case DID_STATIC:
		return 0;

	default:
		return dlg_val[id];
	}
}

static void menudlg_setval(MENUID id, int val)
{
	switch (id) {
    
	case DID_STATIC:
		break;

	default:
		dlg_val[id] = val;
	}
}

static OEMCHAR	skipstr[16];

static void menudlg_settext(MENUID id, const char *work) {
  
	switch (id) {
    
	case DID_SKIPSTR:
		milstr_ncpy(skipstr, work, NELEMENTS(skipstr));
		break;
	}
}

static void setskipstr(void) {

	UINT	val;
	OEMCHAR	work[32];

	val = menudlg_getval(DID_SKIPLIGHT);
	if (val > 256) {
		val = 256;
	}
	OEMSPRINTF(work, str_u, val);
	menudlg_settext(DID_SKIPSTR, work);
}


static void dlginit(void) {

	MENUID	id;

	menudlg_setval(DID_Z80A, 1);

	if (xmilcfg.samplingrate < ((11025 + 22050) / 2)) {
		id = DID_RATE11;
	}
	else if (xmilcfg.samplingrate < ((22050 + 44100) / 2)) {
		id = DID_RATE22;
	}
	else {
		id = DID_RATE44;
	}
	menudlg_setval(id, 1);

	menudlg_setval(DID_SKIPLINE, xmilcfg.skipline);
	menudlg_setval(DID_SKIPLIGHT, xmilcfg.skiplight);
	setskipstr();
}

static void dlgupdate(void) {

	UINT	update;
	UINT	val;
	BRESULT	renewalflg;

	update = 0;

	if (menudlg_getval(DID_RATE11)) {
		val = 11025;
	}
	else if (menudlg_getval(DID_RATE44)) {
		val = 44100;
	}
	else {
		val = 22050;
	}
	if (xmilcfg.samplingrate != (UINT16)val) {
		xmilcfg.samplingrate = (UINT16)val;
		update |= SYS_UPDATECFG;
		corestat.soundrenewal = 1;
	}

	renewalflg = FALSE;
	val = menudlg_getval(DID_SKIPLINE);
	if (xmilcfg.skipline != (UINT8)val) {
		xmilcfg.skipline = (UINT8)val;
		renewalflg = TRUE;
	}
	val = menudlg_getval(DID_SKIPLIGHT);
	if (val > 256) {
		val = 256;
	}
	if (xmilcfg.skiplight != (UINT16)val) {
		xmilcfg.skiplight = (UINT16)val;
		renewalflg = TRUE;
	}
	if (renewalflg) {
		pal_reset();
		update |= SYS_UPDATECFG;
	}

	sysmng_update(update);
}


// ---

static int menu_index;

struct Config {
	int		type;
	MENUID	id;
	const void	*arg;
	float x,y;
	int r, g, b;
};

static int create_menu(Config *dst)
{
	const int num_items = sizeof(res_cfg)/sizeof(res_cfg[0]);
	const MENUPRM *src = res_cfg;
	float x,y;

	int page = 0;
	int n = 0;

	y = 0;

	for (int i = 0; i < num_items; ++i) {
		if (src[i].type == DLGTYPE_FRAME) {
			++page;
		}

		if (menu_index == page) {
      
			dst[n].type = src[i].type;
			dst[n].id = src[i].id;

			switch(src[i].id) {
	
			case DID_SKIPSTR:
				dst[n].arg = (void *)skipstr;
				break;

			default:
				dst[n].arg = src[i].arg;
			}

			switch (src[i].type) {
			case DLGTYPE_FRAME:
				x = (MAX_MENU_SIZE - strlen((char*)dst[n].arg) * ui_font_width()) / 2;
				break;
	
			case DLGTYPE_CTEXT:
				x = (MAX_MENU_SIZE - strlen((char*)dst[n].arg) * ui_font_width()) / 2;
				break;

			case DLGTYPE_RTEXT:
				x = (MAX_MENU_SIZE - (strlen((char*)dst[n].arg)+2) * ui_font_width());
				break;

			case DLGTYPE_SLIDER:
				x = ui_font_width() * 2;
				break;

			case DLGTYPE_RADIO:
				x = (MAX_MENU_SIZE - strlen((char*)dst[n].arg) * ui_font_width()) / 2;
				break;
	
			default:
			case DLGTYPE_LTEXT:
				x = ui_font_width() * 2;
			}
			dst[n].x = x;
			dst[n].y = y;
			dst[n].r = 255;
			dst[n].g = 255;
			dst[n].b = 255;

			switch (src[i].type) {
	
			case DLGTYPE_SLIDER:
				break;

			case DLGTYPE_CHECK:
			case DLGTYPE_RADIO:
				y += ui_font_height() + 5;
				break;
      
			default:
			case DLGTYPE_LTEXT:
			case DLGTYPE_FRAME:
			case DLGTYPE_CTEXT:
			case DLGTYPE_RTEXT:
				y += ui_font_height() + 5;
			}

			++n;
		}
	}
  
	return n;
}

void dlg_rate(int id) {
  
	switch(id) {
    
	case DID_RATE11:
	case DID_RATE44:
	case DID_RATE22:
		menudlg_setval(DID_RATE11, 0);
		menudlg_setval(DID_RATE22, 0);
		menudlg_setval(DID_RATE44, 0);
		menudlg_setval(id, 1);
		break;
	}
}

int cfg_menu(Config *menu, int num_items)
{
	float x,y;
	float base_width = MAX_MENU_SIZE;
	float base_height = ((menu[num_items-1].y + ui_font_height()) - menu[0].y);
	int max_menu;
	int menu_pos;
	char work[32];

	base_height += ui_font_height() * 2;
  
	max_menu = 0;
	for (int i = 0; i < num_items; ++i) {
    
		switch (menu[i].type) {
      
		case DLGTYPE_RADIO:
		case DLGTYPE_CHECK:
		case DLGTYPE_SLIDER:
			++max_menu;
			break;
		}
	}

	int result = -1;
	int id = 0;
	int sel = 0;
  
	int max_val = 0;
	int min_val = 0;
	int val_index = 0;
  
	float offx = ui_offx() + (ui_width() - base_width) / 2;
	float offy = ui_offy() + (ui_height() - base_height) / 2;

	int fade = 128;
  
	for (;;) {
    
		uisys_task();
    
		if (ui_keyrepeat(JOY_UP)) {

			switch (result) {
			case DID_SKIPLIGHT:
				break;
	
			default:
				if (--sel < 0)
					sel = max_menu - 1;
			}
		} else if (ui_keyrepeat(JOY_DOWN)) {

			switch (result) {
	
			case DID_SKIPLIGHT:
				break;
	
			default:
				if (++sel > (max_menu - 1))
					sel = 0;
			}
      
		} else if (ui_keyrepeat(JOY_RIGHT) || ui_keyrepeat(JOY_RTRIGGER)) {
      
			switch (result) {
	
			case DID_SKIPLIGHT:
				if (++val_index > max_val)
					val_index = max_val;
				break;
	
			default:
				if (++menu_index > 3) {
					menu_index = 3;
				} else {
					return 0;
				}
			}
		} else if (ui_keyrepeat(JOY_LEFT) || ui_keyrepeat(JOY_LTRIGGER)) {
      
			switch (result) {
	
			case DID_SKIPLIGHT:
				if (--val_index < min_val)
					val_index = min_val;
				break;
	
			default:
				if (--menu_index < 1) {
					menu_index = 1;
				} else {
					return 0;
				}
			}
		}
    
		if (ui_keypressed(JOY_A)) {

			switch (id) {
	
			case DID_SKIPLINE:
				menudlg_setval(id, !menudlg_getval(id));
				break;
	
			case DID_RATE11:
			case DID_RATE44:
			case DID_RATE22:
				dlg_rate(id);
				break;

			case DID_SKIPLIGHT:
				if (menudlg_getval(DID_SKIPLINE)) {
					if (result == id) {
						menudlg_setval(id, val_index);
						setskipstr();
						result = -1;
					} else {
						val_index = menudlg_getval(id);
						result = id;
					}
				}
				break;
			}
		}
    
		if (ui_keypressed(JOY_B)) {

			switch (result) {
	
			case DID_SKIPLIGHT:
				result = -1;
				break;

			default:
				if (result < 0)
					return -1;
			}
		}

		if ((fade += 255/60) > 255) fade = 128;
      
		tx_resetfont();
      
		scrnmng_update();
		draw_transquad(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, UI_TR, UI_TR, UI_TR, UI_TR);
		draw_transquad(offx, offy, offx + base_width, offy + base_height, UI_BS, UI_BS, UI_BS, UI_BS);

		OEMSPRINTF(work, "%s (%d/%d)", mstr_cfg, menu_index, 3);
		x = offx + (base_width - strlen(work) * ui_font_width()) / 2;
		y = offy;
		ui_font_draw(x, y, 255, 255, 255, work);

		if (menu_index != 1)
			draw_pointer(offx + ui_font_width(), y, 0);
    
		if (menu_index != 3)
			draw_pointer(offx + base_width - ui_font_width(), y, 1);

		y += ui_font_height() + 5;
		x = offx;

		draw_transquad(offx, y, offx + base_width, y + 1, MAKECOL32(128, 128, 128, 255),
					   MAKECOL32(128, 128, 128, 255), MAKECOL32(128, 128, 128, 255), MAKECOL32(128, 128, 128, 255));

		y += 6;

		menu_pos = 0;
      
		for(int i=0; i<num_items; ++i) {
	
			switch (menu[i].type) {
	
			case DLGTYPE_RADIO:
			case DLGTYPE_CHECK:
				if (menu_pos == sel) {
					draw_transquad(offx, y + menu[i].y, offx + base_width, y + menu[i].y + ui_font_height() + 5, MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade));
	  
					id = menu[i].id;
				}

				switch (menu[i].id) {
	  
				case DID_SKIPLINE:
					if (menudlg_getval(menu[i].id)) {
						draw_pointer(x + menu[i].x - ui_font_width(), y + menu[i].y, 1);
					}
					break;
	  
				default:
					if (menudlg_getval(menu[i].id)) {
						draw_pointer(x + menu[i].x - ui_font_width(), y + menu[i].y, 1);
					}
				}
				ui_font_draw(x + menu[i].x, y + menu[i].y, menu[i].r, menu[i].g, menu[i].b, (const char *)menu[i].arg);
	
				++menu_pos;
				break;
	
			case DLGTYPE_SLIDER:
				if (menu_pos == sel) {
					draw_transquad(offx, y + menu[i].y, offx + base_width, y + menu[i].y + ui_font_height() + 5, MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade), MAKECOL32(255, 0, 0, fade));

					id = menu[i].id;
				}
	
				if (result == menu[i].id) {
					const void *arg = menu[i].arg;
					min_val = (SINT16)(long)arg;
					max_val = (SINT16)((long)arg >> 16);
	  
					menu[i].x = (MAX_MENU_SIZE - strlen(work) * ui_font_width()) / 2;
	  
					if (val_index != min_val) {
						draw_pointer(x + menu[i].x - ui_font_width()*2, y + menu[i].y, 0); 
					}
	  
					sprintf(work, "%3d", val_index);
					ui_font_draw(x + menu[i].x, y + menu[i].y, menu[i].r, menu[i].g, menu[i].b, work);
	  
					if (val_index != max_val) {
						draw_pointer(x + menu[i].x + (strlen(work)+2)*ui_font_width(), y + menu[i].y, 1); 	
					}
				}
	
				++menu_pos;
				break;
			}
		}
      
		for (int i = 0; i < num_items; ++i) {
      
			switch (menu[i].type) {
	
			case DLGTYPE_FRAME:
			case DLGTYPE_LTEXT:
			case DLGTYPE_RTEXT:
				ui_font_draw(x + menu[i].x, y + menu[i].y, menu[i].r, menu[i].g, menu[i].b, (const char *)menu[i].arg);
				break;
			}
		}

		x = ui_offx();
		y = ui_height() - ui_font_height();

		switch (result) {
      
		case DID_SKIPLIGHT:
			ui_font_draw(x, y, 255, 255, 255, "A:Select B:Cancel");
			break;
      
		default:
			ui_font_draw(x, y, 255, 255, 255, "A:Select B:Back");
		}
    
		ta_commit_frame();
	}
}

void dlgcfg()
{
	Config cfgmenu[16];
	int n;

	menu_index = 1;
	memset(dlg_val, 0, sizeof(dlg_val));
  
	dlginit();
  
	for (;;) {
		n = create_menu(cfgmenu);
		if (cfg_menu(cfgmenu, n) < 0)
			break;
	}

	dlgupdate();
}
