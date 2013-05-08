#include "compiler.h"
#include "taskmng.h"
#include "dcsys.h"

void taskmng_exit(void) {

	__dc_avail = false;
}
