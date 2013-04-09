#include "compiler.h"
#include "xmil.h"
#include "pccore.h"
#include "soundmng.h"
#include "sound.h"
#include "dosio.h"
#include "parts.h"
#include <ronin/ronin.h>
#include <ronin/soundcommon.h>

static UINT dsstreambytes;
static UINT8 dsstreamevent;

short temp_sound_buffer[RING_BUFFER_SAMPLES] __attribute__((aligned (32)));


// ---- stream

UINT soundmng_create(UINT rate, UINT ms)
{
	UINT samples;

	stop_sound();
	do_sound_command(CMD_SET_BUFFER(3));
	do_sound_command(CMD_SET_STEREO(1));
  
	switch (rate) {
    
	case 44100:
		do_sound_command(CMD_SET_FREQ_EXP(FREQ_44100_EXP));
		break;
    
	case 11025:
		do_sound_command(CMD_SET_FREQ_EXP(FREQ_11025_EXP));
		break;
    
	case 22050:
		do_sound_command(CMD_SET_FREQ_EXP(FREQ_22050_EXP));
		break;

	default:
		return 0;

	}

	ZeroMemory(temp_sound_buffer, sizeof(temp_sound_buffer));

	samples = read_sound_int(&SOUNDSTATUS->ring_length);
  
	samples /= 2;

	dsstreambytes = samples;

	soundmng_reset();
	return samples;
}

void soundmng_reset(void)
{
	dsstreamevent = (UINT8)-1;
}


void soundmng_destroy(void)
{
	stop_sound();
}

void soundmng_play(void)
{
	if (read_sound_int(&SOUNDSTATUS->mode) == MODE_PAUSE)
		start_sound();
}

void soundmng_stop(void)
{
	if (read_sound_int(&SOUNDSTATUS->mode) == MODE_PLAY)
		stop_sound();
}

extern "C" void *memcpy4s(void *s1, const void *s2, unsigned int n);

static void streamwrite(DWORD pos) {

	const SINT32 *pcm;

	pcm = sound_pcmlock();
  
	if (pcm) {
		satuation_s16(temp_sound_buffer, pcm, 2*SAMPLES_TO_BYTES(dsstreambytes));
	} else {
		ZeroMemory(temp_sound_buffer, SAMPLES_TO_BYTES(dsstreambytes));
	}

	memcpy4s(RING_BUF + pos, temp_sound_buffer, SAMPLES_TO_BYTES(dsstreambytes));
  
	sound_pcmunlock(pcm);
}

void soundmng_sync(void) {
  
	DWORD	pos;

	pos = read_sound_int(&SOUNDSTATUS->samplepos);
  
	if (pos >= dsstreambytes) {
		if (dsstreamevent != 0) {
			dsstreamevent = 0;
			streamwrite(0);
		}
	} else {
		if (dsstreamevent != 1) {
			dsstreamevent = 1;
			streamwrite(dsstreambytes);
		}
	}
}


// ---- pcm

void soundmng_pcmload(UINT num, const OEMCHAR *filename, UINT type)
{
}

void soundmng_pcmvolume(UINT num, int volume)
{
}

BRESULT soundmng_pcmplay(UINT num, BRESULT loop)
{
	return FAILURE;
}

void soundmng_pcmstop(UINT num)
{
}

// ----

BRESULT soundmng_initialize(void)
{
	return SUCCESS;
}


