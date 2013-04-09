#include "compiler.h"
#include "dosio.h"
#include "tmpfile.h"

// ----

static _TMPFILEH fh;

void tmpfile_init(void)
{
	TMPFILEH	ret;
	
	ret = &fh;
	ret->size = 0;
	ret->pos = 0;
	ret->fname[0] = '\0';
	ZeroMemory(ret->ptr, TMPFILESIZE);
}

TMPFILEH tmpfile_open(const char *fname)
{
	TMPFILEH ret;
  
	ret = &fh;
  
	if (!milstr_cmp(ret->fname, fname)) {
		ret->pos = 0;
		return ret;
	}
  
	return TMPFILEH_INVALID;
}

TMPFILEH tmpfile_open_rb(const char *fname)
{
	return tmpfile_open(fname);
}

TMPFILEH tmpfile_create(const char *fname)
{
	TMPFILEH ret;
  
	ret = &fh;
	ret->size = 0;
	ret->pos = 0;
	file_cpyname(ret->fname, fname, MAX_PATH);
	ZeroMemory(ret->ptr, TMPFILESIZE);
  
	return ret;
}

long tmpfile_seek(TMPFILEH hdl, long pos, int method)
{
	long	ret;
  
	ret = 0;
	if (hdl == NULL) {
		goto tmpfile_exit;
	}
  
	switch (method) {
	case 1:
		ret = hdl->pos;
		break;
	case 2:
		ret = hdl->size;
		break;
	}
	ret += pos;
	if (ret < 0) {
		ret = 0;
	} else if (ret > (int)hdl->size) {
		ret = hdl->size;
	}
	hdl->pos = ret;
  
tmpfile_exit:
	return ret;
}

UINT tmpfile_read(TMPFILEH hdl, void *buf, UINT size)
{
	UINT	ret;
  
	ret = 0;
	if (hdl == NULL) {
		goto tmpfile_exit;
	}
	ret = min(size, hdl->size - hdl->pos);
	if (ret == 0) {
		goto tmpfile_exit;
	}
  
	CopyMemory(buf, hdl->ptr + hdl->pos, ret);
	hdl->pos += ret;
  
tmpfile_exit:
	return ret;
}

UINT tmpfile_write(TMPFILEH hdl, const void *buf, UINT size)
{
	UINT	ret;
	UINT	maxsize = TMPFILESIZE;
  
	ret = 0;
	if (hdl == NULL) {
		goto tmpfile_exit;
	}
	ret = min(size, maxsize - hdl->pos);
	if (ret == 0) {
		goto tmpfile_exit;
	}
  
	CopyMemory(hdl->ptr + hdl->pos, buf, ret);
	hdl->pos += ret;
  
	if (hdl->pos > (int)hdl->size) {
		hdl->size = hdl->pos;
	}
  
tmpfile_exit:
	return ret;
}

short tmpfile_close(TMPFILEH hdl)
{
	return 0;
}

UINT tmpfile_getsize(TMPFILEH hdl)
{
	UINT	ret;
  
	ret = 0;
	if (hdl == NULL) {
		goto tmpfile_exit;
	}
	ret = hdl->size;
  
tmpfile_exit:
	return ret;
}


short tmpfile_attr(const char *fname)
{
	return (short)0;
}

void tmpfile_load(const char *fname, void *buf, unsigned int size)
{
	TMPFILEH ret;
  
	ret = &fh;
	ret->size = size;
	ret->pos = 0;
	ZeroMemory(ret->ptr, TMPFILESIZE);
	file_cpyname(ret->fname, fname, MAX_PATH);
	CopyMemory(ret->ptr, buf, size);
}
