
typedef unsigned short		MENUID;
typedef unsigned short		MENUFLG;

enum {
	MENU_DISABLE		= 0x0001,
	MENU_GRAY			= 0x0002,
	MENU_CHECKED		= 0x0004,
	MENU_SEPARATOR		= 0x0008,
	MENU_REDRAW			= 0x1000,
	MENU_NOSEND			= 0x2000,
	MENU_TABSTOP		= 0x4000,
	MENU_DELETED		= 0x8000,
	MENU_STYLE			= 0x0ff0
};

enum {
	DID_STATIC		= 0xffff,
};

typedef struct _smi {
	const OEMCHAR		*string;
	const struct _smi	*child;
	MENUID			id;
	MENUFLG			flag;
} MSYSITEM;

enum {
	DLGTYPE_BASE	= 0,
	DLGTYPE_CLOSE,
	DLGTYPE_BUTTON,
	DLGTYPE_LIST,

	DLGTYPE_SLIDER,

	DLGTYPE_TABLIST,
	DLGTYPE_RADIO,
	DLGTYPE_CHECK,

	DLGTYPE_FRAME,
	DLGTYPE_EDIT,
	DLGTYPE_TEXT,
	DLGTYPE_ICON,
	DLGTYPE_VRAM,
	DLGTYPE_LINE,
	DLGTYPE_BOX,

// 互換用…
	DLGTYPE_LTEXT,
	DLGTYPE_CTEXT,
	DLGTYPE_RTEXT
};

typedef struct {
	int		type;
	MENUID	id;
	MENUFLG	flg;
	const void	*arg;
} MENUPRM;

#define SLIDERPOS(a, b)		(((UINT16)a) | (((UINT16)b) << 16))


#ifdef __cplusplus
extern "C" {
#endif

void sysmenu_menuopen();

#ifdef __cplusplus
}
#endif


#define MAX_MENU_SIZE	(ui_font_width() * 32)
