#include "compiler.h"
#include "pccore.h"
#include "xmil.h"
#include "joymng.h"
#include "dcsys.h"
#include "event.h"

enum {
	JOY_LEFT_BIT	= 0x04,
	JOY_RIGHT_BIT	= 0x08,
	JOY_UP_BIT		= 0x01,
	JOY_DOWN_BIT	= 0x02,
	JOY_BTN1_BIT	= 0x40,
	JOY_BTN2_BIT	= 0x20,
	JOY_BTN3_BIT	= 0x80,
	JOY_BTN4_BIT	= 0x10
};

static	UINT8	joyflag;

REG8 joymng_getstat(void) {

	const int Flag = dc_joyinput;

	joyflag = 0xff;
  
	if (xmiloscfg.bindcur == 0)  {
      
		if(Flag & JOY_LEFT) {
			joyflag &= ~JOY_LEFT_BIT;
		} else if(Flag & JOY_RIGHT) {
			joyflag &= ~JOY_RIGHT_BIT;
		}
		if(Flag & JOY_UP) {
			joyflag &= ~JOY_UP_BIT;
		} else if(Flag & JOY_DOWN) {
			joyflag &= ~JOY_DOWN_BIT;
		}
	}

	if (xmiloscfg.bindbtn == 0) {
		if(Flag & JOY_A) {
			joyflag &= ~JOY_BTN1_BIT;
		}
		if(Flag & JOY_B) {
			joyflag &= ~JOY_BTN2_BIT;
		}
		if(Flag & JOY_X) {
			joyflag &= ~JOY_BTN3_BIT;
		}
		if(Flag & JOY_Y) {
			joyflag &= ~JOY_BTN4_BIT;
		}
	}
  
	return joyflag;
}

