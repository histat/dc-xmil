
// �ǁ[�ł������ʒm�n

enum {
	SYS_UPDATECFG		= 0x0001,
	SYS_UPDATEOSCFG	= 0x0002,
	SYS_UPDATECLOCK	= 0x0004,
	SYS_UPDATERATE	= 0x0008,
	SYS_UPDATESBUF	= 0x0010,
	SYS_UPDATEMIDI	= 0x0020,
	SYS_UPDATESBOARD	= 0x0040,
	SYS_UPDATEFDD		= 0x0080,
	SYS_UPDATEHDD		= 0x0100,
	SYS_UPDATEMEMORY	= 0x0200,
	SYS_UPDATESERIAL1	= 0x0400
};


#ifdef __cplusplus
extern "C" {
#endif

extern	UINT	sys_updates;

#if 0
void sysmng_initialize(void);
void sysmng_update(UINT bitmap);
void sysmng_cpureset(void);
void sysmng_scrnwidth(REG8 mode);
void sysmng_fddaccess(UINT8 cDrv);
void sysmng_fddsync(UINT uFrames);
#else

// �}�N��(�P�Ɋ֐��R�[���������Ȃ�����)
#define	sysmng_initialize()		\
	sys_updates = 0
	
#define	sysmng_update(a)		\
	sys_updates |= (a)
	
#define	sysmng_cpureset()								\
	sys_updates	&= (SYS_UPDATECFG | SYS_UPDATEOSCFG);	\
	sysmng_clockreset()
	
#define	sysmng_scrnwidth(m)
#define sysmng_fddaccess(cDrv)
#define sysmng_fddsync(uFrames)
#endif

#ifndef NODISP
void sysmng_clockreset(void);
void sysmng_dispclock(void);
#else
#define	sysmng_clockreset()
#define sysmng_dispclock()
#endif

#ifdef __cplusplus
}
#endif

