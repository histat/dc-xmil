#include "compiler.h"
#include "dosio.h"


static	OEMCHAR	curpath[MAX_PATH];
static	OEMCHAR	*curfilep = curpath;

#define ISKANJI(c) (((((c) ^ 0x20) - 0xa1) & 0xff) < 0x3c)

// ----

void dosio_init(void) { }
void dosio_term(void) { }

// ファイル操作
FILEH file_open(const OEMCHAR *path)
{
	FILEH ret;
  
	if ((ret = open(path, O_RDONLY)) < 0) {
		return FILEH_INVALID;
	}
	return ret;
}

FILEH file_open_rb(const OEMCHAR *path)
{
	FILEH ret;
  
	ret = file_open(path);
	return ret;
}

FILEH file_create(const OEMCHAR *path)
{
	return FILEH_INVALID;
}

long file_seek(FILEH handle, long pointer, int method)
{
	return lseek(handle, pointer, method);
}

UINT file_read(FILEH handle, void *data, UINT length)
{
	return read(handle, data, length);
}

UINT file_write(FILEH handle, const void *data, UINT length)
{
	return 0;
}

short file_close(FILEH handle)
{
	close(handle);
	return 0;
}

UINT file_getsize(FILEH handle)
{
	return file_size(handle);
}

short file_getdatetime(FILEH handle, DOSDATE *dosdate, DOSTIME *dostime)
{
	return -1;
}

short file_delete(const OEMCHAR *path)
{
	return -1;
}

short file_attr(const OEMCHAR *path)
{
	int fd;
	DIR *dp;

	fd = open(path, O_RDONLY);
	if (fd >= 0) {
		close(fd);
		return FILEATTR_READONLY;
	}
	dp = opendir(path);
	if (dp) {
		closedir(dp);
		return FILEATTR_DIRECTORY;
	}
	return -1;
}

short file_dircreate(const OEMCHAR *path)
{
	return -1;
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

FLISTH file_list1st(const OEMCHAR *dir, FLINFO *fli)
{
	FLISTH ret;
  
	ret = (FLISTH)_MALLOC(sizeof(_FLISTH), "FLISTH");
	if (!ret) {
		return FLISTH_INVALID;
	}
  
	file_cpyname(ret->path, dir, sizeof(ret->path));
	file_setseparator(ret->path, sizeof(ret->path));
	ret->hdl = opendir(ret->path);

	if (!ret->hdl) {
		_MFREE(ret);
		return FLISTH_INVALID;
	}
	if (file_listnext((FLISTH)ret, fli) == SUCCESS) {
		return ret;
	}
	closedir(ret->hdl);
	_MFREE(ret);
	return FLISTH_INVALID;
}

BOOL file_listnext(FLISTH hdl, FLINFO *fli)
{
	OEMCHAR buf[MAX_PATH];
	struct dirent *de;
	DIR *dp;
	int fd;
	UINT32 attr;
	UINT32 size;
  
	de = readdir(hdl->hdl);
	if (!de) {
		return FAILURE;
	}
  
	file_cpyname(buf, hdl->path, sizeof(buf));
	file_catname(buf, de->d_name, sizeof(buf));
  
	ZeroMemory(fli, sizeof(FLINFO));
	attr = 0;
	size = 0;
  
	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		dp = opendir(buf);
		if (!dp) {
			return FAILURE;
		}
		attr |= FILEATTR_DIRECTORY;
		closedir(dp);
	} else {
		size = file_size(fd);
		close(fd);
	}
  
	fli->caps = FLICAPS_SIZE | FLICAPS_ATTR;
	fli->attr = attr;
	fli->size = size;
	file_cpyname(fli->path, de->d_name, sizeof(fli->path));
  
	return SUCCESS;
}

void file_listclose(FLISTH hdl)
{
	if (hdl) {
		closedir(hdl->hdl);
		_MFREE(hdl);
	}
}


OEMCHAR *file_getname(const OEMCHAR *path)
{
	const OEMCHAR	*ret;
  
	ret = path;
	while (*path != '\0') {
		if (!ISKANJI(*path)) {
			if (*path == '/') {
				ret = path + 1;
			}
		} else {
			if (path[1]) {
				path++;
			}
		}
		path++;
	}
	return (OEMCHAR *)ret;
}

void file_cutname(OEMCHAR *path)
{
	OEMCHAR 	*p;
  
	p = file_getname(path);
	p[0] = '\0';
}

OEMCHAR *file_getext(const OEMCHAR *path)
{
	const OEMCHAR	*p;
	const OEMCHAR	*q;

	p = file_getname(path);
	q = NULL;
  
	while (*p != '\0') {
		if (!ISKANJI(*p)) {
			if (*p == '.') {
				q = p + 1;
			}
		} else {
			if (p[1]) {
				p++;
			}
		}
		p++;
	}
	if (!q) {
		q = p;
	}
	return (OEMCHAR *)q;
}

void file_cutext(OEMCHAR *path)
{
	OEMCHAR	*p;
	OEMCHAR	*q;

	p = file_getname(path);
	q = NULL;
  
	while (*p != '\0') {
		if (!ISKANJI(*p)) {
			if (*p == '.') {
				q = p;
			}
		} else {
			if (p[1]) {
				p++;
			}
		}
		p++;
	}
	if (q) {
		*q = '\0';
	}
}

void file_cutseparator(OEMCHAR *path)
{
	int		pos;

	pos = strlen(path) - 1;
	if ((pos > 0) && (path[pos] == '/') && (!milstr_kanji2nd(path, pos))
		&& ((pos != 1) || (path[0] != '/'))) {
		path[pos] = '\0';
	}
}

void file_setseparator(OEMCHAR *path, int maxlen)
{
	int		pos;
  
	pos = strlen(path) - 1;
	if ((pos < 0) ||
		((path[pos] == '/') && (!milstr_kanji2nd(path, pos))) ||
		((pos + 2) >= maxlen)) {
		return;
	}
	path[++pos] = '/';
	path[++pos] = '\0';
}

