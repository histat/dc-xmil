
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
#if defined(SUPPORT_8BPP)
#define	scrnmng_getbpp()		(8)
#elif defined(SUPPORT_16BPP)
#define	scrnmng_getbpp()		(16)
#else
#error Not supportetd
#endif
#define	scrnmng_allflash()
#if defined(SUPPORT_8BPP)
#define	scrnmng_palchanged()	scrnmng.palchanged = TRUE
#else
#define	scrnmng_palchanged()
#endif
RGB16 scrnmng_makepal16(RGB32 pal32);			// pal_get16pal	

// ---

void scrnmng_initialize(void);
BRESULT scrnmng_create(UINT8 scrnmode);
void scrnmng_destroy(void);

void scrnmng_clear(BRESULT logo);

#ifdef __cplusplus
}
#endif

