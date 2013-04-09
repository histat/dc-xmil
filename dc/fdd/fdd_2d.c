#include	"compiler.h"
#include	"dosio.h"
#include	"ramsave.h"
#include	"pccore.h"
#include	"fddfile.h"
#include	"fdd_2d.h"
#include	"fdd_mtr.h"


static const _XDFINFO supportxdf[] = {
			{0,  80, 16, 1, DISKTYPE_2D},
			{0, 154, 26, 1, DISKTYPE_2HD}};


static REG8 fdd2d_seek(FDDFILE fdd, REG8 media, UINT track) {

	if ((media != fdd->inf.xdf.media) || (track >= fdd->inf.xdf.tracks)) {
		return(FDDSTAT_SEEKERR);
	}
	return(0x00);
}

static REG8 fdd2d_read(FDDFILE fdd, REG8 media, UINT track, REG8 sc,
												UINT8 *ptr, UINT *size) {

	UINT	secsize;
	UINT	rsize;
	CCFILEH	fh;
	long	pos;
	BRESULT	b;

	if ((media != fdd->inf.xdf.media) || (track >= fdd->inf.xdf.tracks) ||
		(sc == 0) || (sc > fdd->inf.xdf.sectors)) {
		goto fd2r_err;
	}
	secsize = 1 << (7 + fdd->inf.xdf.n);
	rsize = min(secsize, *size);
	if (ptr) {
		fh = ccfile_open_rb(fdd->fname);
		if (fh == CCFILEH_INVALID) {
			goto fd2r_err;
		}
		pos = ((track * fdd->inf.xdf.sectors) + (sc - 1)) * secsize;
		pos += fdd->inf.xdf.headersize;
		b = (ccfile_seek(fh, pos, FSEEK_SET) != pos) ||
			(ccfile_read(fh, ptr, rsize) != rsize);
		ccfile_close(fh);
		if (b) {
			goto fd2r_err;
		}
	}
	*size = rsize;
	return(0x00);

fd2r_err:
	return(FDDSTAT_RECNFND);
}

static REG8 fdd2d_write(FDDFILE fdd, REG8 media, UINT track, REG8 sc,
												const UINT8 *ptr, UINT size) {

	UINT	secsize;
	CCFILEH	fh;
	long	pos;
	BRESULT	b;

	if ((media != fdd->inf.xdf.media) || (track >= fdd->inf.xdf.tracks) ||
		(sc == 0) || (sc > fdd->inf.xdf.sectors)) {
		return(FDDSTAT_RECNFND | FDDSTAT_WRITEFAULT);
	}
	fh = ccfile_open(fdd->fname);
	if (fh == CCFILEH_INVALID) {
		goto fd2w_err;
	}
	secsize = 1 << (7 + fdd->inf.xdf.n);
	pos = ((track * fdd->inf.xdf.sectors) + (sc - 1)) * secsize;
	size = min(size, secsize);
	b = (ccfile_seek(fh, pos, FSEEK_SET) != pos) ||
		(ccfile_write(fh, ptr, size) != size);
	ccfile_close(fh);
	if (b) {
		goto fd2w_err;
	}
	return(0x00);

fd2w_err:
	return(FDDSTAT_WRITEFAULT);
}

static REG8 fdd2d_crc(FDDFILE fdd, REG8 media, UINT track, UINT num,
												UINT8 *ptr) {

	if ((media != fdd->inf.xdf.media) || (track >= fdd->inf.xdf.tracks) ||
		(num >= fdd->inf.xdf.sectors)) {
		return(FDDSTAT_RECNFND);
	}
	ptr[0] = (UINT8)(track >> 1);
	ptr[1] = (UINT8)(track & 1);
	ptr[2] = (UINT8)(num + 1);
	ptr[3] = fdd->inf.xdf.n;
	ptr[4] = 0;										// CRC(Lo)
	ptr[5] = 0;										// CRC(Hi)
	return(0x00);
}


#if defined(SUPPORT_DISKEXT)
static UINT32 fdd2d_sec(FDDFILE fdd, REG8 media, UINT track, REG8 sc) {

	UINT32	ret;

	if ((media != fdd->inf.xdf.media) || (track >= fdd->inf.xdf.tracks)) {
		return(0);
	}
	if ((sc == 0) || (sc > fdd->inf.xdf.sectors)) {
		ret = fdd->inf.xdf.sectors;
	}
	else {
		ret = sc;
	}
	return((16 << fdd->inf.xdf.sectors) + ret);
}
#endif


// ----

BRESULT fdd2d_set(FDDFILE fdd, const OEMCHAR *fname) {

	short		attr;
	CCFILEH		fh;
	UINT		fdsize;
const _XDFINFO	*xdf;
const _XDFINFO	*xdfterm;
	UINT		size;

	attr = ccfile_attr(fname);
	if (attr & 0x18) {
		return(FAILURE);
	}
	fh = ccfile_open(fname);
	if (fh == CCFILEH_INVALID) {
		return(FAILURE);
	}
	fdsize = ccfile_getsize(fh);
	ccfile_close(fh);

	xdf = supportxdf;
	xdfterm = supportxdf + NELEMENTS(supportxdf);
	while(xdf < xdfterm) {
		size = xdf->tracks;
		size *= xdf->sectors;
		size <<= (7 + xdf->n);
		if (size == fdsize) {
			file_cpyname(fdd->fname, fname, sizeof(fdd->fname));
			fdd->type = DISKTYPE_BETA;
			fdd->protect = (UINT8)(attr & 1);
			fdd->seek = fdd2d_seek;
			fdd->read = fdd2d_read;
			fdd->write = fdd2d_write;
			fdd->crc = fdd2d_crc;
#if defined(SUPPORT_DISKEXT)
			fdd->sec = fdd2d_sec;
#endif
			fdd->inf.xdf = *xdf;
			return(SUCCESS);
		}
		xdf++;
	}
	return(FAILURE);
}

void fdd2d_eject(FDDFILE fdd) {

	(void)fdd;
}

