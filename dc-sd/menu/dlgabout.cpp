#include "compiler.h"
#include "strres.h"
#include "xmilver.h"
#include "pccore.h"
#include "scrnmng.h"
#include "sysmenu.h"
#include "dlgabout.h"
#include "dlgabout.h"
#include "icon.h"
#include "dcsys.h"
#include "event.h"
#include "ui.h"
#include <ronin/ta.h>


extern Icon icon;

static const OEMCHAR mstr_about[] = OEMTEXT("About");

void dlgabout()
{
	char work[32];
	const char str_auth[] = "Yui/Punyu/Studio Milmake";
	float x,y;

	tx_resetwork();
	icon.create_texture();
  
	milstr_ncpy(work, str_xmil, NELEMENTS(work));
	milstr_ncat(work, str_space, NELEMENTS(work));
	milstr_ncat(work, xmilversion, NELEMENTS(work));

	float base_width = MAX_MENU_SIZE;
	float base_height = (ui_font_height() + 5) * 3 + (32 + 5) + 6;

	float offx = ui_offx() + (ui_width() - base_width) / 2;
	float offy = ui_offy() + (ui_height() - base_height) / 2;

	tx_resetfont();
  
	scrnmng_update();
	draw_transquad(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, UI_TR, UI_TR, UI_TR, UI_TR);
	draw_transquad(offx, offy, offx + base_width, offy + base_height, UI_BS, UI_BS, UI_BS, UI_BS);

	x = offx + (base_width - strlen(mstr_about) * ui_font_width()) / 2;
	y = offy;
	ui_font_draw(x, y, 255, 255, 255, mstr_about);
	y += ui_font_height() + 5;

	draw_transquad(offx, y, offx + base_width, y + 1, MAKECOL32(128, 128, 128, 255),
				   MAKECOL32(128, 128, 128, 255), MAKECOL32(128, 128, 128, 255), MAKECOL32(128, 128, 128, 255));

	y += 6;
  
	x = offx + (base_width - strlen(work) * ui_font_width()) / 2;
	ui_font_draw(x, y, 255, 255, 255, work);
	y += ui_font_height() + 5;

	x = offx + (base_width - 32) / 2;
	icon.drawIcon(x, y);
	y += 32 + 5;

	x = offx + (base_width - strlen(str_auth) * ui_font_width()) / 2;
	ui_font_draw(x, y, 255, 255, 255, str_auth);

	x = ui_offx();
	y = ui_height() - ui_font_height();
  
	ui_font_draw(x, y, 255, 255, 255, "B:Back");
    
	ta_commit_frame();
  
	for (;;) {
		uisys_task();

		if(ui_keypressed(JOY_B))
			break;
	}
}
