#include "firestring_internal.h"

int firestring_estr_strcat(struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long l;
	l = strlen(s);
	if (l + f->l > f->a)
		return 1;
	memcpy(&f->s[f->l],s,l);
	f->l += l;
	return 0;
}

