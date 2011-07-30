#include "firestring_internal.h"

int firestring_estr_strcpy(struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long l;
	l = strlen(s);
	if (f->a < l)
		return 1;
	f->l = l;
	memcpy(f->s,s,l);
	return 0;
}

