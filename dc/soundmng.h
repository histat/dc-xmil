
enum {
	SOUND_PCMSEEK		= 0,
	SOUND_PCMSEEK1		= 1,

	SOUND_MAXPCM
};


#ifdef __cplusplus
extern "C" {
#endif

UINT soundmng_create(UINT rate, UINT ms);
void soundmng_destroy(void);
void soundmng_reset(void);
void soundmng_play(void);
void soundmng_stop(void);
void soundmng_sync(void);

BRESULT soundmng_pcmplay(UINT num, BRESULT loop);
void soundmng_pcmstop(UINT num);

#ifdef __cplusplus
}
#endif


// ---- for windows

BRESULT soundmng_initialize(void);

void soundmng_pcmload(UINT num, const OEMCHAR *filename, UINT type);
void soundmng_pcmvolume(UINT num, int volume);
