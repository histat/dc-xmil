#include "compiler.h"
#include "xmil.h"
#include "mousemng.h"
#include "dcsys.h"
#include "event.h"

static MOUSEMNG	mousemng;

REG8 mousemng_getstat(SINT16 *x, SINT16 *y, BRESULT clear)
{
	*x = mousemng.x;
	*y = mousemng.y;
	if (clear) {
		mousemng.x = 0;
		mousemng.y = 0;
	}
	return mousemng.btn ^ 3;		// å„Ç≈èCê≥ÇµÇÈ
}


// ----

void mousemng_initialize(void)
{
	ZeroMemory(&mousemng, sizeof(mousemng));
	mousemng.btn = uPD8255A_LEFTBIT | uPD8255A_RIGHTBIT;
}

void mousemng_sync(void)
{
	mousemng.x += dc_mouseaxis1;
	mousemng.y += dc_mouseaxis2;
}

BOOL mousemng_buttonevent(UINT event)
{
	switch (event) {
    
	case MOUSEMNG_LEFTDOWN:
		mousemng.btn &= ~(uPD8255A_LEFTBIT);
		break;
    
	case MOUSEMNG_LEFTUP:
		mousemng.btn |= uPD8255A_LEFTBIT;
		break;
    
	case MOUSEMNG_RIGHTDOWN:
		mousemng.btn &= ~(uPD8255A_RIGHTBIT);
		break;
    
	case MOUSEMNG_RIGHTUP:
		mousemng.btn |= uPD8255A_RIGHTBIT;
		break;
	}
  
	return TRUE;
}
