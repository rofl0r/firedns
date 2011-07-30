#include "firestring_internal.h"

long firestring_snprintf(char * restrict const out, const long size, const char * restrict const format, ...) {
	va_list ap;
	struct firestring_estr_t o;
	va_start(ap,format);
	o.s = out;
	o.l = 0;
	o.a = size - 1;
	firestring_estr_vsprintf(&o,format,ap);
	va_end(ap);
	firestring_estr_0(&o);
	return o.l;
}

