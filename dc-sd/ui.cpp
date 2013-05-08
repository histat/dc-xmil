#include "compiler.h"
#include "sysmng.h"
#include "scrnmng.h"
#include "soundmng.h"
#include "dcsys.h"
#include "event.h"
#include "ui.h"
#include <ronin/ta.h>
#include <ronin/dc_time.h>


KeyList dc_keylist;

static void *romfont_half_addr;

static void* get_romfont_addr()
{
    void *addr;
	__asm__ (
		"jsr @%1\n\t"
		"mov #0,r1\n\t"
		"mov r0,%0"
		:"=r"(addr)
		:"r"(*(void**)0x8c0000b4)
		:"r0","r1","r2","r3","r4","r5","r6","r7","pr"
		);
	return addr;
}

void ui_init(void)
{
	KeyList *kp = &dc_keylist;
  
	kp->vkUp = JOY1_UP;
	kp->vkDown = JOY1_DOWN;
	kp->vkLeft = JOY1_LEFT;
	kp->vkRight = JOY1_RIGHT;
	kp->vkA = JOY1_A;
	kp->vkB = JOY1_B;
	kp->vkX = JOY1_X;
	kp->vkY = JOY1_Y;

	tx_initialize();

	romfont_half_addr = get_romfont_addr();
}

static int cJoy;
static int pJoy;
static unsigned int repeatTime;

void uisys_task()
{
	Event ev;
	int Flag = 0;

	if (PollEvent(ev)) {
		switch (ev.type) {
      
		case EVENT_JOYBUTTONDOWN:
			Flag = ev.jbutton.button;
			break;
      
		case EVENT_JOYBUTTONUP:
			pJoy = 0;
			repeatTime = 0;
			break;

		case EVENT_KEYDOWN:
			switch (ev.key.keycode) {

			case KBD_S1: case KBD_S2:
				Flag |= JOY_START;
				break;
	  
			case 0x52: case 0x60:
				Flag |= JOY_UP;
				break;

			case 0x51: case 0x5a:
				Flag |= JOY_DOWN; 
				break;

			case 0x50: case 0x5c:
				Flag |= JOY_LEFT;
				break;

			case 0x4f: case 0x5e:
				Flag |= JOY_RIGHT; 
				break;
	
			case 0x04:
				Flag |= JOY_A;
				break;

			case 0x05:
				Flag |= JOY_B;
				break;

			case 0x1b:
				Flag |= JOY_X;
				break;

			case 0x1c:
				Flag |= JOY_Y;
				break;
			}

			break;
      
		case EVENT_KEYUP:
			pJoy = 0;
			repeatTime = 0;
			break;
      
		case EVENT_MOUSEMOTION:
		case EVENT_MOUSEBUTTONDOWN:
		case EVENT_MOUSEBUTTONUP:
		case EVENT_JOYAXISMOTION:
			break;
      
		case EVENT_QUIT:
			break;
		}
	}

	cJoy = Flag & 0xffff;
  
	if (cJoy) {
		repeatTime = Timer() + USEC_TO_TIMER(1000000/60*30);
		pJoy = cJoy;
	}
}

bool ui_keypressed(int code)
{
	return (cJoy & code) == code;
}

bool ui_keyrepeat(int code)
{
	if ((cJoy & code) == code)
		return true;

	if ((pJoy & code) == code && repeatTime < Timer()) {
		repeatTime = Timer() + USEC_TO_TIMER(1000000/60*10);
		return true;
	}

	return false;
}


// ----

#define ALLOC_SCREEN_SIZE (SCREEN_WIDTH*SCREEN_HEIGHT*2*4)
#define ALLOC_FONT_SIZE (512*512*2)
#define ALLOC_WORK_SIZE (512*512*2)

static void *screen_tex;
static unsigned char *screen_ptr;

static void *font_tex;
static unsigned char *font_ptr;

static void *work_tex;
static unsigned char *work_ptr;


void *tx_getscreen(int size)
{
	unsigned char *ret = screen_ptr;
	screen_ptr += (size+15) & ~15;
	return (void*)ret;
}

void tx_resetfont()
{
	ta_sync();
  
	font_ptr = (unsigned char *)font_tex;
}

void *tx_getfont(int size)
{
	unsigned char *ret = font_ptr;
	font_ptr += (size+15) & ~15;
	return (void*)ret;
}

void tx_resetwork()
{
	ta_sync();
  
	work_ptr = (unsigned char *)work_tex;
}

void *tx_getwork(int size)
{
	unsigned char *ret = work_ptr;
	work_ptr += (size+15) & ~15;
	return (void*)ret;
}

void tx_initialize()
{
	screen_tex =  ta_txalloc(ALLOC_SCREEN_SIZE);
	screen_ptr = (unsigned char *)screen_tex;
  
	font_tex = ta_txalloc(ALLOC_FONT_SIZE);
	tx_resetfont();

	work_tex = ta_txalloc(ALLOC_WORK_SIZE);
	tx_resetwork();
}

// ----


static void draw_half_char(unsigned short *dst, int yalign, int pos)
{
	int i,j;
	int bits;
	unsigned char *s = ((unsigned char*)romfont_half_addr)+pos*(12*24/8);

	for (i=0; i<12; ++i) {
		bits = *s++ <<16;
		bits |= *s++ << 8;
		bits |= *s++ << 0;
		for (j=0; j<12; ++j, bits<<=1)
			if (bits & 0x800000) {
				dst[j] = 0xffff;
				dst[j+1] = 0xffff;

				dst[j+yalign] = 0xffff;
				dst[j+yalign+1] = 0xffff;
			}
		dst += yalign;
		for (j=0; j<12; ++j, bits<<=1)
			if (bits & 0x800000) {
				dst[j] = 0xffff;
				dst[j+1] = 0xffff;

				dst[j+yalign] = 0xffff;
				dst[j+yalign+1] = 0xffff;
			}
    
		dst += yalign;
	}
}

static void draw_wide_char(unsigned short *dst, int yalign, int pos)
{
  
	int i,j;
	int bits;
	unsigned char *s = ((unsigned char*)romfont_half_addr)+288*36+pos*(24*24/8);

	for(i=0; i<24; ++i) {
		bits = *s++ <<16;
		bits |= *s++ << 8;
		bits |= *s++ << 0;
		for(j=0; j<24; ++j, bits<<=1)
			if(bits & 0x800000) {
				dst[j] = 0xffff;
				dst[j+1] = 0xffff;

				dst[j+yalign] = 0xffff;
				dst[j+yalign+1] = 0xffff;
			}
		dst += yalign;
	}
}

static int sjis2jis(int sjis)
{
	int l,h;
  
	l = sjis & 0xff;
	h = (sjis>>8) & 0xff;

	if (l < 0x81 || l > 0xfc || l > 0x9f && l < 0xe0)
		return 0;

	l -= (l <= 0x9f) ? 0x71 : 0xb1;
	l = (l << 1) + 1;
	if (h > 0x7f) --h;
	if (h >= 0x9e) {
		h -= 0x7d;
		++l;
	} else {
		h -= 0x1f;
	}
    
	if (l >= 0x30) l -= (0x30 - 0x28);
  
	return (l - 0x21) * 94 + h - 0x21;
}

static int ascii2off(int ch)
{
	int c;

	if (ch < 33 || ch == 127)
		c = 96;
	else if (ch >= 33 && ch <= 126)
		c = ch - 32;
	else if (ch == 128)
		c = 95;
	else if (ch >= 160 && ch <= 255)
		c = ch - 160 + 192;
	else
		c = 96;

	return c;
}
    
void draw_romfont_str(unsigned short *dst, int yalign, const char *str)
{
	int c;
	int ch;
	unsigned char *s = (unsigned char*)str;
  
	while(c=*s) {
    
		if ((c>=0x81 && c<=0x9f) || (c>=0xe0 && c<=0xfc)) {
			ch = s[0]|s[1]<<8;
			s += 2;

			draw_wide_char(dst, yalign, sjis2jis(ch));
			dst += 26;
		}
		else {
			ch = s[0];
			s += 1;

			draw_half_char(dst, yalign, ascii2off(ch));
			dst += 14;
		}
	}
}

void draw_font_polygon(float x, float y,unsigned short *tex, int usize, int w, int u, unsigned int col)
{
	struct polygon_list mypoly;
	struct packed_colour_vertex_list myvertex;

	mypoly.cmd =
		TA_CMD_POLYGON|TA_CMD_POLYGON_TYPE_TRANSPARENT|TA_CMD_POLYGON_SUBLIST|
		TA_CMD_POLYGON_STRIPLENGTH_2|TA_CMD_POLYGON_TEXTURED;
	mypoly.mode1 = TA_POLYMODE1_Z_ALWAYS|TA_POLYMODE1_NO_Z_UPDATE;
	mypoly.mode2 = TA_POLYMODE2_BLEND_SRC_ALPHA|TA_POLYMODE2_BLEND_DST_INVALPHA|
		TA_POLYMODE2_FOG_DISABLED|TA_POLYMODE2_ENABLE_ALPHA|
		TA_POLYMODE2_TEXTURE_MODULATE_ALPHA|TA_POLYMODE2_V_SIZE_32|usize;
	mypoly.texture =
		TA_TEXTUREMODE_ARGB1555|TA_TEXTUREMODE_NON_TWIDDLED|TA_TEXTUREMODE_ADDRESS(tex);

	mypoly.alpha = mypoly.red = mypoly.green = mypoly.blue = 0;
	ta_commit_list(&mypoly);
  
	myvertex.colour = col;
	myvertex.ocolour = 0;
	myvertex.z = 0.5;
	myvertex.x = x;
	myvertex.y = y;
	myvertex.cmd = TA_CMD_VERTEX;
	myvertex.u = 0.0;
	myvertex.v = 0.0;
	ta_commit_list(&myvertex);
  
	myvertex.u = w*(1.0/u);
	myvertex.x = x + w;
	ta_commit_list(&myvertex);
  
	myvertex.u = 0.0;
	myvertex.v = 24 * (1.0/32.0);
	myvertex.x = x;
	myvertex.y = y + 24;
	ta_commit_list(&myvertex);
  
	myvertex.u = w*(1.0/u);
	myvertex.x = x + w;
	myvertex.cmd |= TA_CMD_VERTEX_EOS;
	ta_commit_list(&myvertex);
}

void ui_font_draw(float x, float y,int r, int g, int b, const char *str)
{
	unsigned short *texture;
	int w = strlen(str)*14;
	int u;
	int utex;
  
	for(u=8,utex=0; u<w; u<<=1,utex+=(1<<3));

	texture = (unsigned short *)tx_getfont(u*32*2);
	unsigned short *tex = texture;
	memset(texture, 0 , u*32*2);

	draw_romfont_str(tex, u, str);

	draw_font_polygon(x, y,texture, utex, w, u, MAKECOL32(r, g, b, 255));
}


// ----

#define X_OFFSET 20
#define Y_OFFSET 20

#define FONT_WIDTH 16
#define FONT_HEIGHT 24


int ui_offx()
{
	return X_OFFSET;
}

int ui_offy()
{
	return Y_OFFSET;
}

int ui_width()
{
	return SCREEN_WIDTH - X_OFFSET;
}

int ui_height()
{
	return SCREEN_HEIGHT - Y_OFFSET;
}

int ui_font_width()
{
	return FONT_WIDTH;
}

int ui_font_height()
{
	return FONT_HEIGHT;
}


void display_message(const char *str)
{
	float x,y;
  
	x = ui_offx();
	y = ui_offy() + (ui_height() - ui_font_height()) / 2;

	tx_resetfont();
  
	scrnmng_update();
	draw_transquad(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, UI_TR, UI_TR, UI_TR, UI_TR);
  
	ui_font_draw(x, y, 255, 255, 255, str);
	ta_commit_frame();
}


// ----

void draw_transquad(float x1, float y1, float x2, float y2, unsigned int c1, unsigned int c2, unsigned int c3, unsigned int c4)
{
	struct polygon_list mypoly;
	struct packed_colour_vertex_list myvertex;

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
	myvertex.x = x1;
	myvertex.y = y1;
	myvertex.z = 0.5;
	myvertex.colour = c1;
	myvertex.ocolour = 0;
	ta_commit_list(&myvertex);

	myvertex.colour = c2;
	myvertex.x = x2;
	ta_commit_list(&myvertex);

	myvertex.colour = c3;
	myvertex.x = x1;
	myvertex.y = y2;
	ta_commit_list(&myvertex);

	myvertex.colour = c4;
	myvertex.cmd |= TA_CMD_VERTEX_EOS;
	myvertex.x = x2;
	ta_commit_list(&myvertex);
}

void commit_dummy_transpoly()
{
	struct polygon_list mypoly;

	mypoly.cmd =
		TA_CMD_POLYGON|TA_CMD_POLYGON_TYPE_TRANSPARENT|TA_CMD_POLYGON_SUBLIST|
		TA_CMD_POLYGON_STRIPLENGTH_2|TA_CMD_POLYGON_PACKED_COLOUR;
	mypoly.mode1 = TA_POLYMODE1_Z_ALWAYS|TA_POLYMODE1_NO_Z_UPDATE;
	mypoly.mode2 =
		TA_POLYMODE2_BLEND_SRC_ALPHA|TA_POLYMODE2_BLEND_DST_INVALPHA|
		TA_POLYMODE2_FOG_DISABLED|TA_POLYMODE2_ENABLE_ALPHA;
	mypoly.texture = 0;
	mypoly.red = mypoly.green = mypoly.blue = mypoly.alpha = 0;
	ta_commit_list(&mypoly);
}

void draw_poly(float *p, int cnt)
{
	struct polygon_list mypoly;
	struct packed_colour_vertex_list myvertex;

	mypoly.cmd =
		TA_CMD_POLYGON|TA_CMD_POLYGON_TYPE_TRANSPARENT|TA_CMD_POLYGON_SUBLIST|
		TA_CMD_POLYGON_STRIPLENGTH_2|TA_CMD_POLYGON_GOURAUD_SHADING;
	mypoly.mode1 = TA_POLYMODE1_Z_ALWAYS|TA_POLYMODE1_NO_Z_UPDATE;
	mypoly.mode2 = TA_POLYMODE2_BLEND_SRC_ALPHA|TA_POLYMODE2_BLEND_DST_INVALPHA|
		TA_POLYMODE2_ENABLE_ALPHA|TA_POLYMODE2_FOG_DISABLED;
	mypoly.texture = 0;
	mypoly.alpha = mypoly.red = mypoly.green = mypoly.blue = 0;
	ta_commit_list(&mypoly);
	myvertex.cmd = TA_CMD_VERTEX;
	myvertex.z = 0.5;
	myvertex.colour = 0xffffffff;
	myvertex.ocolour = 0;

	for (int i=0; i<cnt; i+=2) {
		if ((i+2) == cnt) {
			myvertex.cmd |= TA_CMD_VERTEX_EOS;
		}
		myvertex.x = p[i];
		myvertex.y = p[i+1];
		ta_commit_list(&myvertex);
	}
}

void draw_pointer(float x, float y, int type)
{
	float p0[] = {x, y, x, y + FONT_HEIGHT, x - FONT_WIDTH, y + FONT_HEIGHT / 2};
	float p1[] = {x, y, x + FONT_WIDTH, y + FONT_HEIGHT / 2, x, y + FONT_HEIGHT};
   

	switch (type) {
	case 1:
		draw_poly(p1, 6);
		break;
    
	case 0:
	default:
		draw_poly(p0, 6);
	}
}

