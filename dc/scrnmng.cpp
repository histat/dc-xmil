#include "compiler.h"
#include "scrnmng.h"
#include "scrndraw.h"
#include "bmpdata.h"
#include "palettes.h"
#include "makescrn.h"
#include "cmndraw.h"
#include "dcsys.h"
#include "event.h"
#include "ui.h"
#include <ronin/ta.h>
#include "xmilbmp.res"


typedef struct {
	int		width;
	int		height;
//	int		extend;
//	int		multiple;
} SCRNSTAT;


		REG8		scrnmode;
//static	DDRAW		ddraw;
		SCRNMNG		scrnmng;
static	SCRNSTAT	scrnstat;
static	SCRNSURF	scrnsurf;


#if defined(SUPPORT_8BPP)
typedef struct polygon_list pvr_context;

typedef unsigned short Texel16_t;

typedef struct {
	Texel16_t color[256 * 4];
} Codebook_t;

pvr_context context[2][3];
void* texs[2][3];

#define TEXSIZE (256*480+2048)
#define SMALLTEXSIZE (128*480+2048)
static unsigned char tscrn[TEXSIZE] __attribute__((aligned(32)));
#endif

static unsigned char screen[SCREEN_WIDTH*SCREEN_HEIGHT*2] __attribute__((aligned (32)));
#if defined(SUPPORT_16BPP)
static void *screen_tx[2];
#endif
static int buffer = 0;


// ----

#define QACR0 (*(volatile unsigned int *)(void *)0xff000038)
#define QACR1 (*(volatile unsigned int *)(void *)0xff00003c)


//#define USE_DMA

void txr_load_dma(void *src, void *dst, unsigned int count)
{
#ifdef USE_DMA

	unsigned int src_addr = (unsigned int)src;
	unsigned int dst_addr = (unsigned int)dst;
	
	
	*((volatile int*)0xA05F7C80) = 0x6702007F;
	*((volatile int*)0xA05F7C00) = dst_addr;
	*((volatile int*)0xA05F7C04) = src_addr;
	*((volatile int*)0xA05F7C08) = count;
	*((volatile int*)0xA05F7C0C) = 0;
	*((volatile int*)0xA05F7C10) = 0;
	*((volatile int*)0xA05F7C14) = 1;
	*((volatile int*)0xA05F7C18) = 1; // kick off

	while(*((volatile int*)0xA05F7C18) != 0);
	
#else
	
	unsigned int *s = (unsigned int *)src;
	unsigned int *d = (unsigned int *)(void *)
		(0xe0000000 | (((unsigned long)dst) & 0x03ffffe0));
	QACR0 = ((((unsigned int)dst)>>26)<<2)&0x1c;
	QACR1 = ((((unsigned int)dst)>>26)<<2)&0x1c;

	count >>= 6;
	
	while (count--) {
		d[0] = *s++;
		d[1] = *s++;
		d[2] = *s++;
		d[3] = *s++;
		asm("pref @%0" : : "r" (s+16));
		d[4] = *s++;
		d[5] = *s++;
		d[6] = *s++;
		d[7] = *s++;
		asm("pref @%0" : : "r" (d));
		d += 8;
		d[0] = *s++;
		d[1] = *s++;
		d[2] = *s++;
		d[3] = *s++;
		asm("pref @%0" : : "r" (s+16));
		d[4] = *s++;
		d[5] = *s++;
		d[6] = *s++;
		d[7] = *s++;
		asm("pref @%0" : : "r" (d));
		d += 8;
	}

#endif
}


void memcpy64(void *src, void *dst, int cnt)
{
	unsigned int *s = (unsigned int *)src;
	unsigned int *d = (unsigned int *)(void *)
		(0xe0000000 | (((unsigned long)dst) & 0x03ffffe0));
	QACR0 = ((((unsigned int)dst)>>26)<<2)&0x1c;
	QACR1 = ((((unsigned int)dst)>>26)<<2)&0x1c;

	cnt >>= 6;
	
	while (cnt--) {
		d[0] = *s++;
		d[1] = *s++;
		d[2] = *s++;
		d[3] = *s++;
		asm("pref @%0" : : "r" (s+16));
		d[4] = *s++;
		d[5] = *s++;
		d[6] = *s++;
		d[7] = *s++;
		asm("pref @%0" : : "r" (d));
		d += 8;
		d[0] = *s++;
		d[1] = *s++;
		d[2] = *s++;
		d[3] = *s++;
		asm("pref @%0" : : "r" (s+16));
		d[4] = *s++;
		d[5] = *s++;
		d[6] = *s++;
		d[7] = *s++;
		asm("pref @%0" : : "r" (d));
		d += 8;
	}
}

#if defined(SUPPORT_8BPP)
static void copy_codebook(Codebook_t* cb, void* tex)
{
	memcpy64(cb, tex, sizeof(*cb));
}

//width in bytes, must be multiple of 32
static void copy_area(unsigned char *fb, size_t readstride, void* tex, size_t writestride, size_t width, size_t lines)
{
	unsigned char *texb = (unsigned char *)tex + sizeof(Codebook_t);
	while(lines--) {
		memcpy64(fb, texb, width);
		
		texb += writestride;
		fb += readstride;
	}
}

static void dma_area(unsigned char *fb, void* tex, size_t width)
{
	unsigned char *texb = (unsigned char *)tex;
	
	txr_load_dma(fb, texb, width + sizeof(Codebook_t));
}


static void copy_640_8(unsigned char * src, void ** dst, Codebook_t* cb, int lines)
{
	copy_codebook(cb, tscrn);
	
	copy_area(src,     640, tscrn, 256, 256, lines);
	dma_area(tscrn, dst[0], 256*lines);
	
	copy_area(src+256, 640, tscrn, 256, 256, lines);
	dma_area(tscrn, dst[1], 256*lines);
	
	copy_area(src+512, 640, tscrn, 128, 128, lines);
	dma_area(tscrn, dst[2], 128*lines);
}


static void draw_quad(float x, float y, float z, float tw, float th, float u0, float v0, float u1, float v1)
{
	struct packed_colour_vertex_list myvertex;

	myvertex.cmd = TA_CMD_VERTEX;
	myvertex.colour = 0;
	myvertex.ocolour = 0;
	myvertex.x = x;
	myvertex.y = y;
	myvertex.z = z;
	myvertex.u = u0;
	myvertex.v = v0;
	ta_commit_list(&myvertex);

	myvertex.x = x;
	myvertex.y = y+th;
	myvertex.z = z;
	myvertex.u = u0;
	myvertex.v = v1;
	ta_commit_list(&myvertex);


	myvertex.x = x+tw;
	myvertex.y = y;
	myvertex.z = z;
	myvertex.u = u1;
	myvertex.v = v0;
	ta_commit_list(&myvertex);

	myvertex.cmd |= TA_CMD_VERTEX_EOS;
	myvertex.x = x+tw;
	myvertex.y = y+th;
	myvertex.z = z;
	myvertex.u = u1;
	myvertex.v = v1;
	ta_commit_list(&myvertex);
}


static void draw_640_8(pvr_context * context, float x, float y, float z)
{
#if 0
	ta_commit_list(&context[0]);
	draw_quad(x, y, z,
			  256, 512,
			  0, 0,
			  1, 512*(1.0/1024));
	
	ta_commit_list(&context[1]);
	draw_quad(x+256, y, z,
			  256, 512,
			  0, 0,
			  1, 512*(1.0/1024));
		

	ta_commit_list(&context[2]);
	draw_quad(x+512, y, z,
			  128, 512,
			  0, 0,
			  1, 512*(1.0/1024));
#else
	ta_commit_list(&context[0]);
	draw_quad(x, y, z,
			  256, scrnstat.height,
			  0, 0,
			  1, scrnstat.height*(1.0/1024));
	
	ta_commit_list(&context[1]);
	draw_quad(x+256, y, z,
			  256, scrnstat.height,
			  0, 0,
			  1, scrnstat.height*(1.0/1024));
		

	ta_commit_list(&context[2]);
	draw_quad(x+512, y, z,
			  128, scrnstat.height,
			  0, 0,
			  1, scrnstat.height*(1.0/1024));
#endif
}


/*
	converts a 256 color palette to a pvr2 codebook designed to allow use of 8 bit texel data
	*pal should point to a 256 entry palette of 2 byte texels
*/
#if 0
static void Palette8BToCodebookLinear(Texel16_t pal[256], Codebook_t *cb)
{
	int i = 256;
	while(i--) {
		cb->color[i*4 + 0] = pal[i];
		cb->color[i*4 + 1] = pal[i];
		cb->color[i*4 + 2] = pal[i];
		cb->color[i*4 + 3] = pal[i];
	}
}
#endif

//MUST be aligned to 8 byte boundry, but 32 byte bountry is better
Codebook_t cb __attribute__((aligned(32)));


void make_vq_texture()
{
	int i,j;
	int tex_u;
	pvr_context *p;

	for(i=0; i<3; ++i) {
		for(j = 0; j < 2; j++) {

			texs[j][i] = tx_getscreen(i==2 ? SMALLTEXSIZE : TEXSIZE);
			p = &context[j][i];
		
			p->cmd =
				TA_CMD_POLYGON|TA_CMD_POLYGON_TYPE_OPAQUE|TA_CMD_POLYGON_SUBLIST|
				TA_CMD_POLYGON_STRIPLENGTH_2|TA_CMD_POLYGON_TEXTURED|TA_CMD_POLYGON_PACKED_COLOUR;
			p->mode1 = TA_POLYMODE1_Z_GREATEREQUAL;
			tex_u = (i == 2 ? TA_POLYMODE2_U_SIZE_512 : TA_POLYMODE2_U_SIZE_1024);
			p->mode2 =
				TA_POLYMODE2_BLEND_SRC|TA_POLYMODE2_FOG_DISABLED|
				tex_u|TA_POLYMODE2_V_SIZE_1024;
			p->texture =
				TA_TEXTUREMODE_RGB565|TA_TEXTUREMODE_NON_TWIDDLED|
				TA_TEXTUREMODE_VQ_COMPRESSION|
				TA_TEXTUREMODE_ADDRESS(texs[j][i]);
	
			p->alpha = p->red = p->green = p->blue = 0;
		}
	}
}
#endif

#if defined(SUPPORT_16BPP)
static void palcnv(CMNPAL *dst, const RGB32 *src, UINT pals, UINT bpp)
{
	UINT	i;

	if (bpp == 16) {
		for (i=0; i<pals; ++i) {
			dst[i].pal16 = ((src[i].p.r & 0xf8) << 8) |
				((src[i].p.g & 0xfc) << 3) |
				(src[i].p.b >> 3);
		}
	}
}

static void bmp16draw(void *bmp, UINT8 *dst, int width, int height,int xalign, int yalign)
{
	CMNVRAM	vram;
  
	vram.ptr = dst;
	vram.width = width;
	vram.height = height;
	vram.xalign = xalign;
	vram.yalign = yalign;
	vram.bpp = 16;
	cmndraw_bmp16(&vram, bmp, palcnv, CMNBMP_CENTER | CMNBMP_MIDDLE);
}
#endif

static void paletteinit(void) {

	scrnmng.palchanged = 0;
}

static void paletteset(void) {

	UINT	i;

	unsigned short pal;
	Codebook_t *p = &cb;
	
	if (xmil_palettes) {
		for (i=0; i<xmil_palettes; i++) {
			pal = (xmil_pal32[i].p.r & 0xf8) << 8
				| (xmil_pal32[i].p.g & 0xfc) << 3
				| xmil_pal32[i].p.b >> 3;
			
			p->color[i*4 + 0] = pal;
			p->color[i*4 + 1] = pal;
			p->color[i*4 + 2] = pal;
			p->color[i*4 + 3] = pal;

		}
	}
}


// ----

void scrnmng_initialize(void)
{
	scrnstat.width = 640;
	scrnstat.height = 400;
}

BRESULT scrnmng_create(UINT8 mode) {

//	int				height;
	UINT			bitcolor;

	
	ZeroMemory(&scrnmng, sizeof(scrnmng));
	
	
#ifdef USE_DMA
	*((volatile int*)0xA05F6884) = 0;
	*((volatile int*)0xA05F6888) = 1;
#endif
	
#if defined(SUPPORT_8BPP)
	bitcolor = 8;
#elif defined(SUPPORT_16BPP)
	bitcolor = 16;
#else
	goto scre_err;
#endif
	
	if 	(bitcolor == 8) {
#if defined(SUPPORT_8BPP)
		paletteinit();

		make_vq_texture();
#else
		goto scre_err;
#endif
		}
		
	else if (bitcolor == 16) {
#if defined(SUPPORT_16BPP)
		*(volatile unsigned int*)(0xa05f80e4) = SCREEN_WIDTH >> 5; //for stride

		int i;
		for (i=0; i<2; ++i) {
			screen_tx[i] = tx_getscreen(SCREEN_WIDTH*SCREEN_HEIGHT*2);
		}
#else
		goto scre_err;
#endif
	}

	scrnmng.bpp = (UINT8)bitcolor;
	scrnsurf.bpp = bitcolor;

	buffer = 0;

#if defined(SUPPORT_8BPP)
	ZeroMemory(screen, SCREEN_WIDTH*SCREEN_HEIGHT*2);

	scrnmng_update();
	commit_dummy_transpoly();
	ta_commit_frame();
#elif defined(SUPPORT_16BPP)	
	scrnmng_clear(TRUE);
#endif
  
	return(SUCCESS);

scre_err:
	scrnmng_destroy();
	return(FAILURE);
}

void scrnmng_destroy(void)
{
}

#if 0
void scrnmng_querypalette(void) {				// ddraws_palette

	reportf("%s:\n",__func__);
}
#endif

RGB16 scrnmng_makepal16(RGB32 pal32)
{
	return (pal32.p.r & 0xf8) << 8 | (pal32.p.g & 0xfc) << 3 | pal32.p.b >> 3;
}

void scrnmng_setheight(int posy, int height)
{
	scrnstat.height = height;
}

const SCRNSURF *scrnmng_surflock(void)
{
	scrnsurf.ptr = (UINT8 *)screen;  
	scrnsurf.width = scrnstat.width;
	scrnsurf.height = scrnstat.height;
	scrnsurf.xalign = scrnmng.bpp >> 3;
	scrnsurf.yalign = SCREEN_WIDTH*scrnsurf.xalign;
//	scrnsurf.bpp = scrnmng.bpp;

	return(&scrnsurf);
}

void scrnmng_surfunlock(const SCRNSURF *surf)
{
	buffer = !buffer;

#if defined(SUPPORT_8BPP)

	if (scrnmng.palchanged) {
		scrnmng.palchanged = FALSE;
		paletteset();
	}

	copy_640_8(screen, texs[buffer], &cb, surf->height);
#elif defined(SUPPORT_16BPP)
	
	memcpy64(screen, screen_tx[buffer], surf->width*surf->height*2);
#endif
}

void scrnmng_update(void)
{
	int x = (SCREEN_WIDTH - scrnstat.width) / 2;
	int y = (SCREEN_HEIGHT - scrnstat.height) / 2;
	float z = 0.5;

#if defined(SUPPORT_8BPP)	

	ta_begin_frame();
	
	draw_640_8(context[buffer], x, y, z);
	
	ta_commit_end();
#elif defined(SUPPORT_16BPP)
	
	struct polygon_list mypoly;
	struct packed_colour_vertex_list myvertex;

	mypoly.cmd =
		TA_CMD_POLYGON|TA_CMD_POLYGON_TYPE_OPAQUE|TA_CMD_POLYGON_SUBLIST|
		TA_CMD_POLYGON_STRIPLENGTH_2|TA_CMD_POLYGON_TEXTURED|TA_CMD_POLYGON_PACKED_COLOUR;
	mypoly.mode1 = TA_POLYMODE1_Z_ALWAYS|TA_POLYMODE1_NO_Z_UPDATE;
	mypoly.mode2 =
		TA_POLYMODE2_BLEND_SRC|TA_POLYMODE2_FOG_DISABLED|TA_POLYMODE2_TEXTURE_REPLACE|
		TA_POLYMODE2_U_SIZE_1024|TA_POLYMODE2_V_SIZE_1024;
	mypoly.texture =
		TA_TEXTUREMODE_RGB565|TA_TEXTUREMODE_STRIDE|TA_TEXTUREMODE_NON_TWIDDLED|
		TA_TEXTUREMODE_ADDRESS(screen_tx[buffer]);
	mypoly.alpha = mypoly.red = mypoly.green = mypoly.blue = 0;
	
	ta_begin_frame();
	ta_commit_list(&mypoly);
  
	myvertex.cmd = TA_CMD_VERTEX;
	myvertex.colour = 0;
	myvertex.ocolour = 0;
	
	myvertex.x = x;
	myvertex.y = y;
	myvertex.z = z;
	myvertex.u = 0.0;
	myvertex.v = 0.0;
	ta_commit_list(&myvertex);
	
	myvertex.x = x;
	myvertex.y = y + scrnstat.height;
	myvertex.z = z;
	myvertex.u = 0.0;
	myvertex.v = scrnstat.height * (1.0/1024);
	ta_commit_list(&myvertex);
	
	myvertex.x = x + scrnstat.width;
	myvertex.y = y;
	myvertex.z = z;
	myvertex.u = scrnstat.width * (1.0/1024);
	myvertex.v = 0.0;
	ta_commit_list(&myvertex);

	myvertex.cmd |= TA_CMD_VERTEX_EOS;
	myvertex.x = x + scrnstat.width;
	myvertex.y = y + scrnstat.height;
	myvertex.z = z;
	myvertex.u = scrnstat.width * (1.0/1024);
	myvertex.v = scrnstat.height * (1.0/1024);
	ta_commit_list(&myvertex);
	
	ta_commit_end();
#else
	ta_begin_frame();
	ta_commit_end();
#endif
}

#if defined(SUPPORT_16BPP)
void scrnmng_clear(BOOL logo)
{
	void	*bmp;
	UINT8	*p;
	UINT8	*q;
	int	y;
	int	x;

	bmp = NULL;
	if (logo) {
		bmp = (void *)bmpdata_solvedata(xmil_bmp);
	}
	p = (UINT8 *)screen;
	q = p;
	y = SCREEN_HEIGHT;
	do {
		x = SCREEN_WIDTH;
		do {
			*(UINT16 *)q = 0;
			q += 2;
		} while (--x);
	} while (--y);
	bmp16draw(bmp, p, SCREEN_WIDTH, SCREEN_HEIGHT, 2, SCREEN_WIDTH*2);
	if (bmp) {
		_MFREE(bmp);
	}

	memcpy64(screen, screen_tx[buffer], SCREEN_WIDTH*SCREEN_HEIGHT*2);
	

	scrnmng_update();
	commit_dummy_transpoly();
	ta_commit_frame();
}
#endif
