#include "firestring_internal.h"

int firestring_estr_starts(const struct firestring_estr_t * restrict const f, const char * restrict const s) {
	return firestring_estr_estarts(f,&ESTR_D((char *)s));
}

