
typedef int	FILEH;
#define FILEH_INVALID -1

typedef struct {
	char		path[MAX_PATH];
	DIR		*hdl;
} _FLISTH, *FLISTH;
#define FLISTH_INVALID NULL

enum {
	FSEEK_SET	= SEEK_SET,
	FSEEK_CUR	= SEEK_CUR,
	FSEEK_END	= SEEK_END
};


enum {
	FILEATTR_READONLY	= 0x01,
	FILEATTR_HIDDEN	= 0x02,
	FILEATTR_SYSTEM	= 0x04,
	FILEATTR_VOLUME	= 0x08,
	FILEATTR_DIRECTORY	= 0x10,
	FILEATTR_ARCHIVE	= 0x20
};

enum {
	FLICAPS_SIZE		= 0x0001,
	FLICAPS_ATTR		= 0x0002,
	FLICAPS_DATE		= 0x0004,
	FLICAPS_TIME		= 0x0008
};

typedef struct {
	UINT16	year;		// cx
	UINT8		month;		// dh
	UINT8		day;		// dl
} DOSDATE;

typedef struct {
	UINT8		hour;		// ch
	UINT8		minute;		// cl
	UINT8		second;		// dh
} DOSTIME;

typedef struct {
	UINT		caps;
	int		size;
	UINT32	attr;
	DOSDATE	date;
	DOSTIME	time;
	char		path[MAX_PATH];
} FLINFO;


#ifdef __cplusplus
extern "C" {
#endif

											// DOSIO:関数の準備
void dosio_init(void);
void dosio_term(void);
											// ファイル操作
FILEH file_open(const OEMCHAR *path);
FILEH file_open_rb(const OEMCHAR *path);
FILEH file_create(const OEMCHAR *path);
long file_seek(FILEH handle, long pointer, int method);
UINT file_read(FILEH handle, void *data, UINT length);
UINT file_write(FILEH handle, const void *data, UINT length);
short file_close(FILEH handle);
UINT file_getsize(FILEH handle);
short file_getdatetime(FILEH handle, DOSDATE *dosdate, DOSTIME *dostime);
short file_delete(const OEMCHAR *path);
short file_attr(const OEMCHAR *path);
short file_dircreate(const OEMCHAR *path);

											// カレントファイル操作
void file_setcd(const OEMCHAR *exepath);
OEMCHAR *file_getcd(const OEMCHAR *path);
FILEH file_open_c(const OEMCHAR *path);
FILEH file_open_rb_c(const OEMCHAR *path);
FILEH file_create_c(const OEMCHAR *path);
short file_delete_c(const OEMCHAR *path);
short file_attr_c(const OEMCHAR *path);

FLISTH file_list1st(const OEMCHAR *dir, FLINFO *fli);
BOOL file_listnext(FLISTH hdl, FLINFO *fli);
void file_listclose(FLISTH hdl);

#define	file_cpyname(a, b, c)	milstr_ncpy(a, b, c)
#define	file_catname(a, b, c)	milstr_ncat(a, b, c)
#define	file_cmpname(a, b)	milstr_cmp(a, b)
OEMCHAR *file_getname(const OEMCHAR *path);
void file_cutname(OEMCHAR *path);
OEMCHAR *file_getext(const OEMCHAR *path);
void file_cutext(OEMCHAR *path);
void file_cutseparator(OEMCHAR *path);
void file_setseparator(OEMCHAR *path, int maxlen);

#ifdef __cplusplus
}
#endif

