
struct KeyList {
	short	vkUp;
	short	vkDown;
	short	vkLeft;
	short	vkRight;
	short	vkA;
	short	vkB;
	short	vkX;
	short	vkY;
};

enum {
	JOY1_C		= 0xa0,
	JOY1_B,
	JOY1_A,
	JOY1_START,
	JOY1_UP,
	JOY1_DOWN,
	JOY1_LEFT,
	JOY1_RIGHT,
	JOY1_Z,
	JOY1_Y,
	JOY1_X,
	JOY1_D,
	JOY1_RTRIGGER,
	JOY1_LTRIGGER,
};

#define MAKECOL15(r, g, b)	((r) << 10 | (g) << 5 | (b) | 0x8000)
#define MAKECOL32(r, g, b, a)	((r) << 16 | (g) << 8 | (b) << 0 | (a) << 24)

#define UI_TR MAKECOL32(0, 0, 0, 204)
#define UI_BS MAKECOL32(0, 0, 255, 204)


#ifdef __cplusplus
extern "C" {
#endif

void ui_init(void);
void uisys_task();
bool ui_keypressed(int code);
bool ui_keyrepeat(int code);

void tx_initialize();
void *tx_getscreen(int size);
void tx_resetfont();
void *tx_getfont(int size);
void tx_resetwork();
void *tx_getwork(int size);

void draw_romfont_str(unsigned short *dst, int yalign, const char *str);
void draw_font_polygon(float x, float y,unsigned short *tex, int usize, int w, int u, unsigned int col);
void ui_font_draw(float x, float y,int r, int g, int b, const char *str);  
void display_message(const char *str);

int ui_offx();
int ui_offy();
int ui_width();
int ui_height();
int ui_font_width();
int ui_font_height();

void draw_transquad(float x1, float y1, float x2, float y2, unsigned int c1, unsigned int c2, unsigned int c3, unsigned int c4);

void commit_dummy_transpoly();
  
void draw_pointer(float x, float y, int type);
  
#ifdef __cplusplus
}
#endif


