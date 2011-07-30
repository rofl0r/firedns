#include "firestring_internal.h"

int firestring_estr_ends(const struct firestring_estr_t * restrict const f, const char * restrict const s) {
	return firestring_estr_eends(f,&ESTR_D((char *)s));
}

