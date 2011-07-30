#include "firestring_internal.h"

long firestring_estr_sprintfe(struct firestring_estr_t * restrict const o, const struct firestring_estr_t * restrict const format, ...) {
	va_list ap;
	va_start(ap,format);
	return firestring_estr_vsprintfe(o,format,ap);
}

