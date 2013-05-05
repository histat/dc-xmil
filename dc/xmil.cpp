#include "compiler.h"
#include "xmil.h"
#include "dosio.h"
#include "joymng.h"
#include "mousemng.h"
#include "scrnmng.h"
#include "soundmng.h"
#include "sysmng.h"
#include "scrndraw.h"
#include "dckbd.h"
#include "ini.h"
#include "z80core.h"
#include "pccore.h"
#include "iocore.h"
#include "timing.h"
#include "debugsub.h"
#include "makescrn.h"
#include "diskdrv.h"
#include "fdd_ini.h"
#include "x1f.h"
#include "taskmng.h"
#include "sysmenu.h"
#include "dcsys.h"
#include "event.h"
#include "ui.h"
#include "ramsave.h"
#include "dc_softkbd.h"
#include <ronin/ronin.h>


bool __dc_avail;

unsigned int dc_savedtimes;

unsigned short dc_joyinput;

short dc_mouseaxis1;
short dc_mouseaxis2;


static char modulefile[MAX_PATH] = "/";

XMILOSCFG	xmiloscfg = {
	0, 0,
	0, 0,
	0, 0};

#define	MAX_FRAMESKIP		4

struct tagXmilMain
{
	UINT uFrameCount;
	UINT uWaitCount;
	UINT uFrameMax;
};
typedef struct tagXmilMain		XMILMAIN;
typedef struct tagXmilMain		*PXMILMAIN;


static void framereset(XMILMAIN &xmm, UINT uCount)
{
	xmm.uFrameCount = 0;
	sysmng_dispclock();
	sysmng_fddsync(uCount);
}

static void processwait(XMILMAIN &xmm, UINT uCount)
{
	if (timing_getcount() >= uCount) {
		timing_setcount(0);
		framereset(xmm, uCount);
	} else {
		Sleep(960);
	}
}

static void exec1frame(XMILMAIN &xmm)
{
	joymng_sync();
	pccore_exec((BRESULT)(xmm.uFrameCount == 0));
	xmm.uFrameCount++;

	mousemng_sync();
	softkbddc_sync();
}



// ----

bool dcsys_task()
{
	enum {
		CMD_MENU = 1,
	};
	int cmd = 0;
  
	Event ev;
	static  unsigned int tick = 0;

	unsigned int tm = Timer() - tick;
	if (tm < USEC_TO_TIMER(1000000/60)) {
		return __dc_avail;
	}
  
	tick += tm;

	int x, y;

	int cJoy = 0;
	static int pJoy;
	static unsigned int repeatTime;

	if (__dc_avail) {

		dc_mouseaxis1 = 0;
		dc_mouseaxis2 = 0;

		while (PollEvent(ev)) {
      
			switch (ev.type) {
	
			case EVENT_KEYDOWN:
				switch (ev.key.keycode) {
				case KBD_S1: case KBD_S2:
					cmd = CMD_MENU;
					break;
	
				default:
					dckbd_keydown(ev.key.keycode);
				}
				break;
	
			case EVENT_KEYUP:
				dckbd_keyup(ev.key.keycode);
				break;
	
			case EVENT_MOUSEMOTION:
				dc_mouseaxis1 += ev.motion.x;
				dc_mouseaxis2 += ev.motion.y;
				break;

			case EVENT_MOUSEBUTTONDOWN:
				switch (ev.button.button) {
				case EVENT_BUTTON_LEFT:
					mousemng_buttonevent(MOUSEMNG_LEFTDOWN);
					break;

				case EVENT_BUTTON_RIGHT:
					mousemng_buttonevent(MOUSEMNG_RIGHTDOWN);
					break;
				}
				break;
      
			case EVENT_MOUSEBUTTONUP:
				switch (ev.button.button) {
				case EVENT_BUTTON_LEFT:
					mousemng_buttonevent(MOUSEMNG_LEFTUP);
					break;

				case EVENT_BUTTON_RIGHT:
					mousemng_buttonevent(MOUSEMNG_RIGHTUP);
					break;
				}
				break;

			case EVENT_JOYAXISMOTION:
				x = 0;
				y = 0;
	
				if (ev.jaxis.axis == 0) {
					x = ev.jaxis.value;
				}
				if (ev.jaxis.axis == 1) {
					y = ev.jaxis.value;
				}

				dc_mouseaxis1 += x;
				dc_mouseaxis2 += y;
	  
				break;

			case EVENT_JOYBUTTONDOWN:

				if (ev.jbutton.button == JOY_START)
					cmd = CMD_MENU;

				if (ev.jbutton.button == JOY_RTRIGGER)
					__skbd_avail = !__skbd_avail;

				if (__skbd_avail && ev.jbutton.button == JOY_A)
					softkbddc_down();

				if (__skbd_avail && ev.jbutton.button == JOY_B)
					__use_bg = !__use_bg;
	
					if (__skbd_avail) {
					cJoy = ev.jbutton.button & 0xffff;
	  
				} else {

					switch (ev.jbutton.button) {
	    
					case JOY_UP:
						dckbd_keydown(JOY1_UP);
						break;
	    
					case JOY_DOWN:
						dckbd_keydown(JOY1_DOWN);
						break;
	    
					case JOY_LEFT:
						dckbd_keydown(JOY1_LEFT);
						break;
	    
					case JOY_RIGHT:
						dckbd_keydown(JOY1_RIGHT);
						break;
	    
					case JOY_A:
						dckbd_keydown(JOY1_A);
						break;
	    
					case JOY_B:
						dckbd_keydown(JOY1_B);
						break;

					case JOY_X:
						mousemng_buttonevent(MOUSEMNG_LEFTDOWN);
						break;
	    
					case JOY_Y:
						mousemng_buttonevent(MOUSEMNG_RIGHTDOWN);
						break;
					}
				}
				break;
	
			case EVENT_JOYBUTTONUP:

				softkbddc_up();
	
				if (__skbd_avail) {

					pJoy = 0;
					repeatTime = 0;

				} else {
	  
					switch (ev.jbutton.button) {
	    
					case JOY_UP:
						dckbd_keyup(JOY1_UP);
						break;
	    
					case JOY_DOWN:
						dckbd_keyup(JOY1_DOWN);
						break;
	    
					case JOY_LEFT:
						dckbd_keyup(JOY1_LEFT);
						break;
	    
					case JOY_RIGHT:
						dckbd_keyup(JOY1_RIGHT);
						break;
	    
					case JOY_A:
						dckbd_keyup(JOY1_A);
						break;
	    
					case JOY_B:
						dckbd_keyup(JOY1_B);
						break;
	    
					case JOY_X:
						mousemng_buttonevent(MOUSEMNG_LEFTUP);
						break;

					case JOY_Y:
						mousemng_buttonevent(MOUSEMNG_RIGHTUP);
						break;
					}
				}
				break;
	
			case EVENT_QUIT:
				__dc_avail = false;
				break;
			}
		}
	}

  
	if (__skbd_avail) {
		dc_joyinput = 0;

		int button = 0;
    
		if (cJoy) {
			repeatTime = Timer() + USEC_TO_TIMER(1000000/60*30);
			pJoy = cJoy;
			button = cJoy;
		} else if (repeatTime < Timer()) {

			button = pJoy;
			repeatTime = Timer() + USEC_TO_TIMER(1000000/60*10);
		}

		softkbddc_send(button);

	} else
		getJoyButton(JOYSTICKID1, &dc_joyinput);

	if (sys_updates & (SYS_UPDATECFG | SYS_UPDATEOSCFG)) {
		initsave();
		sysmng_initialize();
	}

	if (dc_savedtimes && Timer() >= dc_savedtimes) {
		ccfile_term();
	}

	switch (cmd) {
	case CMD_MENU:
		sysmenu_menuopen();
		break;
	}

	scrnmng_update();
	softkbddc_draw();
	ta_commit_frame();
  
	return __dc_avail;
}



int main(void)
{
	XMILMAIN xmilmain;

#ifndef NOSERIAL
	serial_init(57600);
	usleep(20000);
	printf("Serial OK\n");
#endif

	cdfs_init();
	maple_init();
	dc_setup_ta();
	init_arm();
  
	__dc_avail = true;

	dc_savedtimes = 0;
	dc_joyinput = 0;

	dc_mouseaxis1 = 0;
	dc_mouseaxis2 = 0;

  
	ui_init();
  
	dosio_init();

	file_setcd(modulefile);

	ccfile_init();
  
	TRACEINIT();
  
//  keystat_initialize();
  
	mousemng_initialize();

	scrnmng_initialize();

	scrndraw_initialize();

	
	UINT8 scrnmode =  0;
	
	if (scrnmng_create(scrnmode) != SUCCESS) {
		return -1;
	}

	initload();

	
	softkbddc_initialize();

	dckbd_bindinit();
	dckbd_bindcur(xmiloscfg.bindcur);
	dckbd_bindbtn(xmiloscfg.bindbtn);
  
	if (soundmng_initialize() == SUCCESS) {
		soundmng_pcmload(SOUND_PCMSEEK, OEMTEXT("fddseek.wav"), 0);
		soundmng_pcmload(SOUND_PCMSEEK1, OEMTEXT("fddseek1.wav"), 0);
		soundmng_pcmvolume(SOUND_PCMSEEK, xmilcfg.MOTORVOL);
		soundmng_pcmvolume(SOUND_PCMSEEK1, xmilcfg.MOTORVOL);
	}
  
	sysmng_initialize();
	joymng_initialize();
  
	pccore_initialize();
	pccore_reset();

	scrndraw_redraw();


	xmilmain.uFrameCount = 0;
	xmilmain.uWaitCount = 0;
	xmilmain.uFrameMax = 1;

	while(dcsys_task()) {
		if (xmiloscfg.NOWAIT) {
			exec1frame(xmilmain);
			if (xmiloscfg.DRAW_SKIP) {
				// nowait frame skip
				if (xmilmain.uFrameCount >= xmiloscfg.DRAW_SKIP) {
					processwait(xmilmain, 0);
				}
			} else {
				// nowait auto skip
				if (timing_getcount()) {
					processwait(xmilmain, 0);
				}
			}
		} else if (xmiloscfg.DRAW_SKIP) {
			// frame skip
			if (xmilmain.uFrameCount < xmiloscfg.DRAW_SKIP) {
				exec1frame(xmilmain);
			} else {
				processwait(xmilmain, xmiloscfg.DRAW_SKIP);
			}
		} else {
			// auto skip
			if (!xmilmain.uWaitCount) {
				exec1frame(xmilmain);
				const UINT uCount = timing_getcount();
				if (xmilmain.uFrameCount > uCount) {
					xmilmain.uWaitCount = xmilmain.uFrameCount;
					if (xmilmain.uFrameMax > 1) {
						xmilmain.uFrameMax--;
					}
				} else if (xmilmain.uFrameCount >= xmilmain.uFrameMax) {
					if (xmilmain.uFrameMax < MAX_FRAMESKIP) {
						xmilmain.uFrameMax++;
					}
					if (uCount >= MAX_FRAMESKIP) {
						timing_reset();
					} else {
						timing_setcount(uCount - xmilmain.uFrameCount);
					}
					framereset(xmilmain, 0);
				}
			} else {
				processwait(xmilmain, xmilmain.uWaitCount);
				xmilmain.uWaitCount = xmilmain.uFrameCount;
			}
		}
	}

	x1f_close();
	pccore_deinitialize();
  
	scrnmng_destroy();

	if (sys_updates & (SYS_UPDATECFG | SYS_UPDATEOSCFG)) {
		initsave();
	}

	ccfile_term();

  
	TRACETERM();
	dosio_term();
  
#ifdef NOSERIAL
	(*(void(**)(int))0x8c0000e0)(1);
	while (1) { }
#endif
  
	return 0;
}

