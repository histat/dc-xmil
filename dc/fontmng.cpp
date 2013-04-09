#include	"compiler.h"
#include	"fontmng.h"

// ----

void *fontmng_create(int size, UINT type, const char *fontface) {
	return(NULL);
}

void fontmng_destroy(void *hdl) {
}

BOOL fontmng_getsize(void *hdl, const char *string, POINT_T *pt) {
	return(FAILURE);
}

BOOL fontmng_getdrawsize(void *hdl, const char *string, POINT_T *pt) {
	return(FAILURE);
}

// ----

FNTDAT fontmng_get(void *hdl, const char *string) {
	return(NULL);
}




