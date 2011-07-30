#include "firestring_internal.h"

long firestring_printfe(const struct firestring_estr_t * restrict const format, ...) {
	va_list ap;
	long o;
	va_start(ap,format);
	o = firestring_vfprintfe(stdout,format,ap);
	va_end(ap);
	return o;
}

