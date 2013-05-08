#include "compiler.h"
#include "xmil.h"
#include "dckbd.h"
#include "keystat.h"
#include "ui.h"


#define NC	0xff

static UINT8 key106[256] = {

	//                            A     B     C     D      ; 0x00
  	NC,   NC,   NC,   NC,   0x1d, 0x2d, 0x2b, 0x1f,
	//    E     F     G     H     I     J     K     L      ; 0x08
  	0x12, 0x20, 0x21, 0x22, 0x17, 0x23, 0x24, 0x25,
	//    M     N     O     P     Q     R     S     T      ; 0x10
  	0x2f, 0x2e, 0x18, 0x19, 0x10, 0x13, 0x1e, 0x14,
	//    U     V     W     X     Y     Z     1     2      ; 0x18
  	0x16 ,0x2c, 0x11, 0x2a, 0x15, 0x29, 0x01, 0x02,
	//    3     4     5     6     7     8     9     0      ; 0x20
	0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
	//    ENT   ESC   BS    TAB   SPC   -     ^     @      ; 0x28
	0x1c, 0x00, 0x0e, 0x0f, 0x34, 0x0b, 0x0c, 0x1a,  
	//    [           ]     ;     :     XFER  ,     .      ; 0x30
	0x1b, NC,   0x28, 0x26, 0x27, 0x35, 0x30, 0x31,
	//    /     CAPS  f.1   f.2   f.3   f.4   f.5   f.6    ; 0x38
	0x32, 0x81, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	//    f.7   f.8   f.9   f.10  f.11  f.12  PNT          ; 0x40
	0x68, 0x69, 0x6a, 0x6b, NC,   NC,   NC,   NC,
	//    PAUS  INS   HOME  RLUP  DEL   END   RLDN  Å®     ; 0x48
	0x60, 0x38, 0x3e, 0x37, 0x39, 0x3f, 0x36, 0x3c,
	//    Å©    Å´    Å™     <NL>  </>   <*>   <->   <+>    ; 0x50
	0x3b, 0x3d, 0x3a, NC,   0x41, 0x45, 0x40, 0x49,
	//    <ENT> <1>   <2>   <3>   <4>   <5>   <6>   <7>    ; 0x58
	0x1c, 0x4a, 0x4b, 0x4c, 0x46, 0x47, 0x48, 0x42,
	//    <8>   <9>   <0>   <.>   <\>   S3                 ; 0x60
	0x43, 0x44, 0x4e, 0x50, 0x43, NC,   NC,   NC,
	//                                                     ; 0x68
	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,
	//                                                     ; 0x70
	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,
	//                                                     ; 0x78
	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,
	//                                              _      ; 0x80
	NC,   NC,   NC,   NC,   NC,   NC,   NC,   0x33,
	//    KANA  Åè    NFER  KAN                            ; 0x88
	0x82, 0x0d, 0x51, NC,   NC,   NC,   NC,   NC,
	//    CTRL  SFT   ALT   S1  CTRL  SFT   ALT     S2     ; 0x90
	0x74, 0x70, 0x73, NC, 0x74, 0x70, 0x73,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,
	
	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,
	
	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,
	
	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC,

	NC,   NC,   NC,   NC,   NC,   NC,   NC,   NC
};

void dckbd_keydown(UINT8 key) {
  
	UINT8 data;

	data = key106[key];
  
	if (data != NC) {
		keystat_keydown(data);
	}
}

void dckbd_keyup(UINT8 key) {
  
	UINT8 data;

	data = key106[key];
  
	if (data != NC) {
		keystat_keyup(data);
	}
}

// ----

extern KeyList dc_keylist;

typedef struct {
	short	*ptr[4];
} KEYADRS;

typedef struct {
	UINT8	key[4];
} KEYSET;

typedef struct {
	KEYADRS	curadrs;
	KEYADRS	btnadrs;
	KEYSET	curset[2];
	KEYSET	btnset[2];
} PPCBTNTBL;

typedef struct {
	KEYSET	cur;
	KEYSET	btn;
} PPCBTNDEF;


static const PPCBTNTBL ppcbtntbl = {
	{&dc_keylist.vkUp, &dc_keylist.vkDown,
	 &dc_keylist.vkLeft, &dc_keylist.vkRight},
  
	{&dc_keylist.vkA, &dc_keylist.vkB,
	 &dc_keylist.vkX, &dc_keylist.vkY},
  
	{{0x3a, 0x3d, 0x3b, 0x3c},			// cur
	 {0x43, 0x4b, 0x46, 0x48}},			// tenkey
  
	{{0x1c, 0x34,   NC,   NC},		// RET/SP
	 {0x29, 0x2a,   NC,   NC}}};	// ZX

static	PPCBTNDEF	ppcbtndef;

static void getbind(KEYSET *bind, const UINT8 *tbl, const KEYADRS *adrs) {

	int		i;
	int		key;

	for (i=0; i<4; i++) {
		key = (*adrs->ptr[i]) & 0xff;
		bind->key[i] = tbl[key];
	}
}

static void setbind(UINT8 *tbl, const KEYSET *bind, const KEYADRS *adrs) {

	int		i;
	int		key;

	for (i=0; i<4; i++) {
		key = (*adrs->ptr[i]) & 0xff;
		if (tbl[key] != NC) {
			keystat_keyup(tbl[key]);
		}
		tbl[key] = bind->key[i];
	}
}

void dckbd_bindinit(void) {

	getbind(&ppcbtndef.cur, key106, &ppcbtntbl.curadrs);
	getbind(&ppcbtndef.btn, key106, &ppcbtntbl.btnadrs);
}

void dckbd_bindcur(UINT type) {

	const KEYSET	*bind;

	switch(type) {
	case 0:
	default:
		bind = &ppcbtndef.cur;
		break;

	case 1:
		bind = ppcbtntbl.curset + 0;
		break;

	case 2:
		bind = ppcbtntbl.curset + 1;
		break;
	}
	setbind(key106, bind, &ppcbtntbl.curadrs);
}

void dckbd_bindbtn(UINT type) {

	const KEYSET	*bind;

	switch(type) {
	case 0:
	default:
		bind = &ppcbtndef.btn;
		break;

	case 1:
		bind = ppcbtntbl.btnset + 0;
		break;

	case 2:
		bind = ppcbtntbl.btnset + 1;
		break;
	}
	setbind(key106, bind, &ppcbtntbl.btnadrs);
}
