

#define	DATACACHES	2
#define	DATACACHESIZE	1510000

typedef struct {
	BYTE		ptr[DATACACHESIZE] __attribute__((aligned (32)));
	long		pos;
	UINT		size;
	unsigned int	id;
	char		name[MAX_PATH];
} _CCFILEH, *CCFILEH;

#define CCFILEH_INVALID NULL


#ifdef __cplusplus
extern "C" {
#endif

void ccfile_init(void);
void ccfile_term(void);
CCFILEH ccfile_open(const char *fname);
CCFILEH ccfile_open_rb(const char *fname);
CCFILEH ccfile_create(const char *fname);
long ccfile_seek(CCFILEH hdl, long pos, int method);
UINT ccfile_read(CCFILEH hdl, void *buf, UINT size);
UINT ccfile_write(CCFILEH hdl, const void *buf, UINT size);
short ccfile_close(CCFILEH hdl);
UINT ccfile_getsize(CCFILEH hdl);
short ccfile_attr(const char *fname);

#ifdef __cplusplus
}
#endif

