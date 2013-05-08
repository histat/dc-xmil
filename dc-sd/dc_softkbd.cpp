#include "compiler.h"
#include "keystat.h"
#include "dcsys.h"
#include "event.h"
#include "ui.h"
#include "dc_softkbd.h"
#include <ronin/ta.h>



#define MAX_KEYS (13*6)
#define SOFTKBD_ROW (13)
#define SOFTKBD_COLUMN (6)
#define SOFTKBD_W (14*3)
#define SOFTKBD_H (26)
#define MARGIN (2)


#define SOFTKEY_NC 0xff

BOOL __skbd_avail;
BOOL __use_bg;
static UINT8	sendkey;

static const char keyname[] = {
	"Esc\0""F1\0""F2\0""F3\0""F4\0""F5\0""F6\0""F7\0""F8\0""F9\0""F10\0""Xfr\0""Nfr\0"
 
	"1\0""2\0""3\0""4\0""5\0""6\0""7\0""8\0""9\0""0\0""-\0""^\0""\x80\0"
 
	"Tab\0""Q\0""W\0""E\0""R\0""T\0""Y\0""U\0""I\0""O\0""P\0""@\0""[\0"
 
	"Cap\0""A\0""S\0""D\0""F\0""G\0""H\0""J\0""K\0""L\0"";\0"":\0""]\0"
 
	"Sft\0""Z\0""X\0""C\0""V\0""B\0""N\0""M\0"",\0"".\0""/\0""_\0""Ent\0"
 
	"Ctl\0""Alt\0""Spc\0""Hom\0""End\0""\xb6\xc5\0""Rdn\0""Rup\0""Bs\0""Å™\0""Å´\0""Å©\0""Å®"
};


static const UINT8 __keytbl [] = {
  
	0x00,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x35,0x51,
  
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,
  
	0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,
  
	0x81,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
  
	0x70,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x1c,
  
	0x74,0x73,0x34,0x3e,0x3f,0x82,0x36,0x37,0x0e,0x3a,0x3d,0x3b,0x3c,
};


// --------------------------------------------------------

struct Label {
	unsigned short *tex;
	int w;
	int u;
	int usize;
};

static Label label[MAX_KEYS];
static int skbd_pos;
static int skbd_cur;
static int shift,alt,ctrl;


static void create(const char *str, Label *dst)
{
	unsigned short *texture;
	int w = strlen(str)*14;
	int u;
	int usize;
  
	for(u=8,usize=0; u<w; u<<=1,usize+=(1<<3));

	texture = (unsigned short *)ta_txalloc(u*32*2);
	unsigned short *tex = texture;
	memset(texture, 0 , u*32*2);

	draw_romfont_str(tex, u, str);
  
	dst->tex = texture;
	dst->usize = usize;
	dst->w = w;
	dst->u = u;
}

static void draw(Label *src, float x, float y, unsigned int col)
{
	draw_font_polygon(x, y, src->tex, src->usize, src->w, src->u, col);
}

void softkbddc_initialize()
{
	uint i;

	skbd_cur = 0;
	skbd_pos = 0;
	sendkey = SOFTKEY_NC;

	__use_bg = true;
	
	const char *str = keyname;
  
	for(i=0;i<MAX_KEYS; ++i) {
		create(str, &label[i]);
		str += strlen(str) + 1;
	}
}

void softkbddc_send(int button)
{
	switch (button) {
    
	case JOY_UP:
		if ((skbd_cur - SOFTKBD_ROW) < 0)
			skbd_cur += 5*13;
		else
			skbd_cur -= 13;
		break;
    
	case JOY_DOWN:
		if ((skbd_cur + SOFTKBD_ROW) > (MAX_KEYS-1))
			skbd_cur -= 5*13;
		else
			skbd_cur += 13;
		break;
    
	case JOY_LEFT:
		if (--skbd_cur < 0)
			skbd_cur = MAX_KEYS -1;

		break;
    
	case JOY_RIGHT:
		if (++skbd_cur == MAX_KEYS)
			skbd_cur = 0;
		break;
	}
}

void softkbddc_down()
{
	UINT8	key;

	key = __keytbl[skbd_cur];
  
#ifndef NOSERIAL
//	reportf("%s: 0x%x\n",__func__, key);
#endif

	switch(key) {
	case 0x70:
    
		shift = !shift;
		if(shift)
			keystat_keydown(key);
		else
			keystat_keyup(key);

		key = SOFTKEY_NC;
		break;
    
	case 0x73:
    
		alt = !alt;
		if(alt)
			keystat_keydown(key);
		else
			keystat_keyup(key);

		key = SOFTKEY_NC;
		break;
    
	case 0x74:

		ctrl = !ctrl;
		if(ctrl)
			keystat_keydown(key);
		else
			keystat_keyup(key);

		key = SOFTKEY_NC;
		break;
	}

	if (key != SOFTKEY_NC) {
		keystat_keydown(key);
		sendkey = key;
	}
}

void softkbddc_up()
{
	if (sendkey != SOFTKEY_NC) {
		keystat_keyup(sendkey);
		sendkey = SOFTKEY_NC;
	}
}

void softkbddc_sync()
{
	if(__skbd_avail) {

		if (skbd_pos > 120)
			return;
    
		skbd_pos += 10;

	} else {

		if (skbd_pos < 0)
			return;
    
		skbd_pos -= 10;
	}
}

void softkbddc_draw()
{
	if (skbd_pos > 0) {

		int i;
		unsigned int col;
		float x,y;
		int w;
		float pos_x = 640 - (640 * sin(skbd_pos * PI / 320));
		float pos_y = (480 - (26 * 6)) / 2;
    
		int fade = 120 - ((skbd_pos > 120) ? 120 : skbd_pos);

		x = pos_x;
		y = pos_y;
    
		int c = 0;
    
		for(i=0; i<MAX_KEYS; ++i) {

			col = 0;
			w = SOFTKBD_W;
      
			switch (i) {
			case SOFTKBD_ROW*4:
				if(shift) {
					col = MAKECOL32(0, 255, 255, 255-fade);
				}
				break;
	
			case SOFTKBD_ROW*5:
				if(ctrl) {
					col = MAKECOL32(0, 255, 255, 255-fade);
				}
				break;
	
			case SOFTKBD_ROW*5+1:
				if(alt) {
					col = MAKECOL32(0, 255, 255, 255-fade);
				}
				break;
			}
      
			if (skbd_cur == i) {
				col = MAKECOL32(0xe0, 0, 0, 255-fade);
				draw_transquad(x, y, x+w, y+24, col, col, col, col);
			} else {
				col = MAKECOL32(0, 0, 0xe0, 255-fade);
				
				if (__use_bg)
					draw_transquad(x, y, x+w, y+24, col, col, col, col);
			}

			draw(&label[i], x, y, MAKECOL32(255, 255, 255, 255-fade));

			x += (SOFTKBD_W+MARGIN);

			++c;
      
			if(c >= SOFTKBD_ROW) {
				x = pos_x;
				y += SOFTKBD_H+MARGIN;
				c = 0;
			}
		}
	}
	else
		commit_dummy_transpoly();
}
