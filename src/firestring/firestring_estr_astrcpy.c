#include "firestring_internal.h"

int firestring_estr_astrcpy(struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long l;
	l = strlen(s);
	firestring_estr_expand(f,f->l + l);
	f->l = l;
	memcpy(f->s,s,l);
	return 0;
}

