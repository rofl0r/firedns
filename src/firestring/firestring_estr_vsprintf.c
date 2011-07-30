#include "firestring_internal.h"

long firestring_estr_vsprintf(struct firestring_estr_t * restrict const o, const char * restrict const format, va_list ap) {
	return firestring_estr_vsprintfe(o,&ESTR_D((char *)format),ap);
}

