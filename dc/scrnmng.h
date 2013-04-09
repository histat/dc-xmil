
enum {
	START_PAL	= 0x00
};

typedef struct {
	UINT8	*ptr;
	int		xalign;
	int		yalign;
	int		width;
	int		height;
	UINT	bpp;
//	int		extend;
} SCRNSURF;

enum {
	SCRNMODE_FULLSCREEN		= 0x01,
	SCRNMODE_SYSHIGHCOLOR	= 0x02,
	SCRNMODE_COREHIGHCOLOR	= 0x04
};

enum {
	SCRNFLAG_FULLSCREEN		= 0x01,
	SCRNFLAG_ENABLE			= 0x80
};

typedef struct {
	UINT8	bpp;
	UINT8	flag;
	UINT8	allflash;
	UINT8	palchanged;
} SCRNMNG;


#ifdef __cplusplus
extern "C" {
#endif

extern	REG8		scrnmode;
extern	SCRNMNG		scrnmng;			// マクロ用
	
//void scrnmng_setwidth(int posx, int width);
void scrnmng_setheight(int posy, int height);	
BRESULT scrnmng_setcolormode(BRESULT fullcolor);
const SCRNSURF *scrnmng_surflock(void);
void scrnmng_surfunlock(const SCRNSURF *surf);
void scrnmng_update();

#define	scrnmng_setcolormode(f)	(SUCCESS)
#define	scrnmng_isfullscreen()
#define	scrnmng_getbpp()		(scrnmng.bpp)
#define	scrnmng_allflash()		scrnmng.allflash = TRUE
#define	scrnmng_palchanged()	scrnmng.palchanged = TRUE

RGB16 scrnmng_makepal16(RGB32 pal32);			// pal_get16pal	

// ---

void scrnmng_initialize(void);						// ddraws_initwindowsize
BRESULT scrnmng_create(UINT8 scrnmode);				// ddraws_InitDirectDraw
void scrnmng_destroy(void);							// ddraws_TermDirectDraw
BRESULT scrnmng_changescreen(REG8 newmode);

void scrnmng_querypalette(void);					// ddraws_palette
void scrnmng_fullscrnmenu(int y);
//void scrnmng_topwinui(void);						// ddraws_topwinui
//void scrnmng_clearwinui(void);						// ddraws_clearwinui
	
#define	USE_PALS		0xc0
#define	START_EXT		(START_PAL + USE_PALS)
#define	EXT_PALS		0x28
#define	TOTAL_PALS		(USE_PALS + EXT_PALS)

#ifdef __cplusplus
}
#endif

