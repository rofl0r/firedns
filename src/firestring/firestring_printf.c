#include "firestring_internal.h"

long firestring_printf(const char * restrict const format, ...) {
	va_list ap;
	long o;
	va_start(ap,format);
	o = firestring_vfprintfe(stdout,&ESTR_D((char *)format),ap);
	va_end(ap);
	return o;
}

