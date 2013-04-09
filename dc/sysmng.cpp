#include "compiler.h"
#include "xmil.h"
#include "sysmng.h"
#include "z80core.h"
#include "pccore.h"
#include "makescrn.h"

UINT	sys_updates;


#ifndef NODISP

//static	OEMCHAR	title[512];
static	OEMCHAR	clock[64];

static struct {
	UINT32	tick;
	UINT32	clock;
	UINT32	draws;
	SINT32	fps;
	SINT32	khz;
} workclock;

void sysmng_clockreset(void) {

	workclock.tick = GETTICK();
	workclock.clock = CPU_CLOCK;
	workclock.draws = drawtime;				// drawcount;
}

static BRESULT workclockrenewal(void) {

	SINT32	tick;

	tick = GETTICK() - workclock.tick;
	if (tick < 2000) {
		return(FALSE);
	}
	workclock.tick += tick;
	workclock.fps = ((drawtime - workclock.draws) * 10000) / tick;
	workclock.draws = drawtime;
	workclock.khz = (CPU_CLOCK - workclock.clock) / tick;
	workclock.clock = CPU_CLOCK;
	return(TRUE);
}

void sysmng_dispclock(void) {

	OEMCHAR	work[256];

	if (workclockrenewal()) {
		OEMSPRINTF(work, OEMTEXT("%2u.%03uMHz %2u.%1uFPS")
				   ,workclock.khz / 1000, workclock.khz % 1000
				   ,workclock.fps / 10, workclock.fps % 10);

		reportf("%s\n",work);
	}
}

#endif
