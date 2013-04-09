#include "compiler.h"
#include "strres.h"
#include "profile.h"
#include "dosio.h"
#include "xmil.h"
#include "ini.h"
#include "pccore.h"
#include "scrnmng.h"
#include "soundmng.h"
#include "dcsys.h"
#include "tmpfile.h"
#include "icon.h"
#include "vmu.h"
#include "ui.h"
#include "xmil_icon.res"


Icon icon;

const char ini_title[] = "Xmillennium";
const char inifile[] = OEMTEXT("xmil.cfg");

enum {
	PFRO_BOOL			= PFTYPE_BOOL + PFFLAG_RO,
	PFMAX_UINT8			= PFTYPE_UINT8 + PFFLAG_MAX,
	PFAND_UINT8			= PFTYPE_UINT8 + PFFLAG_AND,
	PFROAND_HEX32		= PFTYPE_HEX32 + PFFLAG_RO + PFFLAG_AND
};

static const PFTBL iniitem[] = {

	// OSàÀë∂Å`
	{"s_NOWAIT", PFTYPE_BOOL,		&xmiloscfg.NOWAIT,		0},
	{"SkpFrame", PFTYPE_UINT8,		&xmiloscfg.DRAW_SKIP,	0},

	{"pbindcur", PFTYPE_UINT8,		&xmiloscfg.bindcur,		0},
	{"pbindbtn", PFTYPE_UINT8,		&xmiloscfg.bindbtn,		0},


	// xmil
	{"IPL_TYPE", PFMAX_UINT8,		&xmilcfg.ROM_TYPE,		3},
	{"Resolute", PFTYPE_HEX8,		&xmilcfg.DIP_SW,		0},

	{"DispSync", PFTYPE_BOOL,		&xmilcfg.DISPSYNC,		0},
	{"Real_Pal", PFTYPE_BOOL,		&xmilcfg.RASTER,		0},
	{"skipline", PFTYPE_BOOL,		&xmilcfg.skipline,		0},
	{"skplight", PFTYPE_UINT16,		&xmilcfg.skiplight,		0},

	{"SampleHz", PFTYPE_UINT16,		&xmilcfg.samplingrate,	0},
	{"Latencys", PFTYPE_UINT16,		&xmilcfg.delayms,		0},
	{"OPMsound", PFTYPE_BOOL,		&xmilcfg.SOUND_SW,		0},
	{"Seek_Snd", PFTYPE_BOOL,		&xmilcfg.MOTOR,			0},
	{"Seek_Vol", PFMAX_UINT8,		&xmilcfg.MOTORVOL,		100},

	{"MouseInt", PFTYPE_BOOL,		&xmilcfg.MOUSE_SW,		0},
	{"btnRAPID", PFTYPE_BOOL,		&xmilcfg.BTN_RAPID,		0},
	{"btn_MODE", PFTYPE_BOOL,		&xmilcfg.BTN_MODE,		0},

	{"Joystick", PFTYPE_BOOL,		&xmiloscfg.JOYSTICK,	0},
	
	{"Z80_SAVE", PFTYPE_BOOL,		&xmiloscfg.Z80SAVE,		0},
};

static int checked_vm = 0;

void initload(void)
{
	OEMCHAR fname[16];
	unsigned char buf[TMPFILESIZE];
	int size = TMPFILESIZE;
	int res;
	const char *load_title = "Select a VMU to load from";

	icon.loadIcon(xmil_icon);
  
	tmpfile_init();
	milstr_ncpy(fname, inifile, NELEMENTS(fname));
	ZeroMemory(buf, sizeof(buf));

	display_message("Scanning VMU...");
  
	int files = vm_SearchFile(fname, &res);
  
	if (!files) {

		if (!vmu_select(&res, "Select a VMU to create")) {
			checked_vm = -1;
			return;
		}
    
		checked_vm = res;
    
		initsave();
		return;
	}

	if (files == 1 && load_from_vmu(res, fname, buf, &size)) {
      
		tmpfile_load(fname, buf, size);

		profile_iniread(fname, ini_title, iniitem, NELEMENTS(iniitem), NULL);
    
		checked_vm = res;
		return;
	}

	for (;;) {
		if (!vmu_select(&res, load_title)) {
			checked_vm = -1;
			return;
		}
		if(!load_from_vmu(res, fname, buf, &size)) {

			display_message("Failed!");
			drawlcdstring(res, 0, 8, "FAILED!");
		} else {
			tmpfile_load(fname, buf, size);

			profile_iniread(fname, ini_title, iniitem, NELEMENTS(iniitem), NULL);

			checked_vm = res;

			return;
		}
	}
}

void initsave(void)
{
	OEMCHAR fname[16];
	OEMCHAR long_desc[32];
	OEMCHAR short_desc[16];
	unsigned char buf[TMPFILESIZE];
	TMPFILEH fh;
	int size;
	int res;
	const char *save_title = "Select a VMU to save";
  
	if (checked_vm < 0)
		return;

	soundmng_stop();

	milstr_ncpy(fname, inifile, NELEMENTS(fname));
	ZeroMemory(buf, sizeof(buf));
	OEMSPRINTF(long_desc, "%s/System File", ini_title);
	milstr_ncpy(short_desc, ini_title, NELEMENTS(short_desc));

	profile_iniwrite(fname, ini_title, iniitem, NELEMENTS(iniitem), NULL);
  
	fh = tmpfile_open(fname); 
	size = tmpfile_getsize(fh);
  
	if (!size) {
		goto inisave_exit;
	}

	tmpfile_read(fh, buf, size);

	res = checked_vm;
	for (;;) {
		display_message("Saving, don't power off or remove VMU!");
    
		if (!save_to_vmu(res, fname, long_desc, short_desc, buf, size, &icon)) {

			display_message("Failed!");
			drawlcdstring(res, 0, 8, "FAILED!");

			if (!vmu_select(&res, save_title)) {
				checked_vm = -1;
				goto inisave_exit;
			}
		} else {

			display_message("Saved!");
			drawlcdstring(res, 0, 8, "SAVED!");
      
			checked_vm = res;
			goto inisave_exit;
		}
	}
  
inisave_exit:
	soundmng_play();
}
