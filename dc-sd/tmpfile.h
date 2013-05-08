

#define	TMPFILESIZE (128*1024)

typedef struct {
	BYTE ptr[TMPFILESIZE] __attribute__((aligned (32)));
	long	pos;
	UINT size;
	char fname[MAX_PATH];
} _TMPFILEH, *TMPFILEH;

#define TMPFILEH_INVALID NULL


#ifdef __cplusplus
extern "C" {
#endif


void tmpfile_init(void);
TMPFILEH tmpfile_open(const char *fname);
TMPFILEH tmpfile_open_rb(const char *fname);
TMPFILEH tmpfile_create(const char *fname);
long tmpfile_seek(TMPFILEH hdl, long pos, int method);
UINT tmpfile_read(TMPFILEH hdl, void *buf, UINT size);
UINT tmpfile_write(TMPFILEH hdl, const void *buf, UINT size);
short tmpfile_close(TMPFILEH hdl);
UINT tmpfile_getsize(TMPFILEH hdl);
short tmpfile_attr(const char *fname);
void tmpfile_load(const char *fname, void *buf, unsigned int size);


#ifdef __cplusplus
}
#endif
