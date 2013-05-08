#include "compiler.h"
#include "dosio.h"


static	OEMCHAR	curpath[MAX_PATH];
static	OEMCHAR	*curfilep = curpath;

#define ISKANJI(c) (((((c) ^ 0x20) - 0xa1) & 0xff) < 0x3c)

// ----

static FATFS Fatfs;
static FILINFO _fi;

static char lfn[_MAX_LFN + 1];

void dosio_init(void) {

	f_mount(0, &Fatfs);

	_fi.lfname = lfn;
	_fi.lfsize = sizeof(lfn);
}

void dosio_term(void) {

	f_mount(0, NULL);
}

// ファイル操作
FILEH file_open(const OEMCHAR *path)
{
	FRESULT res;
	FILEH fi;

	fi = (FILEH)malloc(sizeof(FIL));
	
	res = f_open(fi, path, FA_READ | FA_WRITE | FA_OPEN_EXISTING);

	if (res != FR_OK) {

		printf("%s failed %s %d\n", __func__, path, res);
		return FILEH_INVALID;
	}
	
	return(fi);
}

FILEH file_open_rb(const OEMCHAR *path)
{
	FRESULT res;
	FILEH fi;

	fi = (FILEH)malloc(sizeof(FIL));
	
	res = f_open(fi, path, FA_READ | FA_OPEN_EXISTING);
	
	if (res != FR_OK) {

		printf("%s failed %s %d\n", __func__, path, res);
		return FILEH_INVALID;
	}
	
	return(fi);
}

FILEH file_create(const OEMCHAR *path)
{
	FRESULT res;
	FILEH fi;

	fi = (FILEH)malloc(sizeof(FIL));
	
	res = f_open(fi, path, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
	
	if (res != FR_OK) {

		printf("%s failed %s %d\n", __func__, path, res);
		return FILEH_INVALID;
	}
	
	return(fi);
}

long file_seek(FILEH handle, long pointer, int method) {
	long	ret;
	FRESULT res;
  
	ret = 0;
	
	switch (method) {
	case 1:
		ret = f_tell(handle);
		break;
	case 2:
		ret = f_size(handle);
		break;
	}
	ret += pointer;

	if (ret < 0) {
		ret = 0;
	} else if (ret > (long)f_size(handle)) {
		ret = f_size(handle);
	}
	
	res = f_lseek (handle, ret);

	if (res != FR_OK)
		return -1;

	return ret;
	
}

UINT file_read(FILEH handle, void *data, UINT length)
{
	UINT	readsize;
	
	if(f_read (handle, data, length, &readsize) != FR_OK) {

		printf("%s failed 0x%x 0x%x\n", __func__, length, readsize);
		return(0);
	}

	return(readsize);
}

UINT file_write(FILEH handle, const void *data, UINT length)
{
	UINT	writesize;
	
	if (length) {
		if (f_write (handle, data, length, &writesize) == FR_OK) {

			return(writesize);
		}
		else {
			f_lseek (handle, f_size(handle));
		}
	}

	return(0);
}

short file_close(FILEH handle)
{
	f_close (handle);

	free(handle);

	return 0;
}

UINT file_getsize(FILEH handle)
{
	return f_size(handle);
}

static BOOL cnvdatetime(FILINFO *fno, DOSDATE *dosdate, DOSTIME *dostime)
{
	if (dosdate) {
		dosdate->year = (fno->fdate >> 9) + 1980;
		dosdate->month = (fno->fdate >> 5) & 0x0f;
		dosdate->day = (fno->fdate >> 0) & 0x0f;
	}
	if (dostime) {
		dostime->hour = (fno->ftime >> 11);
		dostime->minute = (fno->ftime >> 5) & 0x3f;
		dostime->second = (fno->ftime >> 0) & 0x0f;
	}
	return SUCCESS;
}

short file_getdatetime(const char *path, DOSDATE *dosdate, DOSTIME *dostime)
{
	FILINFO *fi = &_fi;
	
	if ((f_stat(path, fi) == FR_OK)
	 && (cnvdatetime(fi, dosdate, dostime)))
		return 0;
	return -1;
}

short file_delete(const OEMCHAR *path)
{
	return(f_unlink(path)? FAILURE:SUCCESS);
}

short file_attr(const OEMCHAR *path)
{
	FILINFO *fi = &_fi;
	
	f_stat(path, fi);

	return fi->fattrib;
}

short file_dircreate(const OEMCHAR *path)
{
	return(f_mkdir(path) ?FAILURE:SUCCESS);
}


// カレントファイル操作
void file_setcd(const OEMCHAR *exepath)
{
	file_cpyname(curpath, exepath, sizeof(curpath));
	curfilep = file_getname(curpath);
	*curfilep = '\0';
}

OEMCHAR *file_getcd(const OEMCHAR *path)
{
	*curfilep = '\0';
	file_catname(curpath, path, sizeof(curpath));
	return curpath;
}

FILEH file_open_c(const OEMCHAR *path)
{
	*curfilep = '\0';
	file_catname(curpath, path, sizeof(curpath));
	return file_open(curpath);
}

FILEH file_open_rb_c(const OEMCHAR *path)
{
	*curfilep = '\0';
	file_catname(curpath, path, sizeof(curpath));
	return file_open_rb(curpath);
}

FILEH file_create_c(const OEMCHAR *path)
{
	*curfilep = '\0';
	file_catname(curpath, path, sizeof(curpath));
	return file_create(curpath);
}

short file_delete_c(const OEMCHAR *path)
{
	*curfilep = '\0';
	file_catname(curpath, path, sizeof(curpath));
	return file_delete(curpath);
}

short file_attr_c(const OEMCHAR *path)
{
	*curfilep = '\0';
	file_catname(curpath, path, sizeof(curpath));
	return file_attr(curpath);
}

static BRESULT setflist(FILINFO *fi, FLINFO *fli) {

	fli->caps = FLICAPS_SIZE | FLICAPS_ATTR | FLICAPS_DATE | FLICAPS_TIME;
	fli->size = fi->fsize;
	fli->attr = fi->fattrib;
	cnvdatetime(fi, &fli->date, &fli->time);
	char *fn = *fi->lfname ? fi->lfname : fi->fname;
	file_cpyname(fli->path, fn, NELEMENTS(fli->path));

	return(SUCCESS);
}

FLISTH file_list1st(const OEMCHAR *dir, FLINFO *fli) {
	FRESULT res;
	FILINFO *fi = &_fi;
	
	OEMCHAR path[MAX_PATH];
	file_cpyname(path, dir, NELEMENTS(path));
//	file_setseparator(path, NELEMENTS(path));
//	printf("file_list1st %s\n", path);

	FLISTH dj = (FLISTH)malloc(sizeof(DIR));
	
	res = f_opendir(dj, path);
	if (res == FR_OK) {
		while ((f_readdir(dj, fi) == FR_OK) && fi->fname[0]) {
			if (setflist(fi, fli) == SUCCESS) {
				return(dj);
			}
		}
	}
	return(FLISTH_INVALID);
}

BRESULT file_listnext(FLISTH hdl, FLINFO *fli) {

	FILINFO *fi = &_fi;
	char *fn;
	
	if ((f_readdir(hdl, fi) != FR_OK) || fi->fname[0] == 0) {
		return(FAILURE);
	}

//	fn = *fi->lfname ? fi->lfname : fi->fname;
//	printf("file_listnext %s\n", fn);
	
	if (setflist(fi, fli) == SUCCESS) {
		return(SUCCESS);
	}
	printf("file_listnext fail %s\n", fi->fname);
	return(FAILURE);
}

void file_listclose(FLISTH hdl) {

	free(hdl);
}


OEMCHAR *file_getname(const OEMCHAR *path) {

const OEMCHAR	*ret;
	int			csize;

	ret = path;
	while((csize = milstr_charsize(path)) != 0) {
		if ((csize == 1) &&
			((*path == '\\') || (*path == '/') || (*path == ':'))) {
			ret = path + 1;
		}
		path += csize;
	}
	return((OEMCHAR *)ret);
}

void file_cutname(OEMCHAR *path) {

	OEMCHAR	*p;

	p = file_getname(path);
	p[0] = '\0';
}

OEMCHAR *file_getext(const OEMCHAR *path) {

const OEMCHAR	*p;
const OEMCHAR	*q;
	int			csize;

	p = file_getname(path);
	q = NULL;
	while((csize = milstr_charsize(p)) != 0) {
		if ((csize == 1) && (*p == '.')) {
			q = p + 1;
		}
		p += csize;
	}
	if (q == NULL) {
		q = p;
	}
	return((OEMCHAR *)q);
}

void file_cutext(OEMCHAR *path) {

	OEMCHAR	*p;
	OEMCHAR	*q;
	int		csize;

	p = file_getname(path);
	q = NULL;
	while((csize = milstr_charsize(p)) != 0) {
		if ((csize == 1) && (*p == '.')) {
			q = p;
		}
		p += csize;
	}
	if (q) {
		*q = '\0';
	}
}

void file_cutseparator(OEMCHAR *path) {

	int		pos;

	pos = OEMSTRLEN(path) - 1;
	if ((pos > 0) &&							// 2文字以上でー
		(path[pos] == '\\') &&					// ケツが \ でー
		(!milstr_kanji2nd(path, pos)) &&		// 漢字の2バイト目ぢゃなくてー
		((pos != 1) || (path[0] != '\\')) &&	// '\\' ではなくてー
		((pos != 2) || (path[1] != ':'))) {		// '?:\' ではなかったら
		path[pos] = '\0';
	}
}

void file_setseparator(OEMCHAR *path, int maxlen) {

	int		pos;

	pos = OEMSTRLEN(path) - 1;
	if ((pos < 0) ||
		((pos == 1) && (path[1] == ':')) ||
		((path[pos] == '\\') && (!milstr_kanji2nd(path, pos))) ||
		((pos + 2) >= maxlen)) {
		return;
	}
	path[++pos] = '\\';
	path[++pos] = '\0';
}

