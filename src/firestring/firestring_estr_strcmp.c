#include "firestring_internal.h"

int firestring_estr_strcmp(const struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long i;
	if (f->l != strlen(s))
		return 1;
	for (i = 0; i < f->l; i++)
		if (f->s[i] !=s[i])
			return 1;
	return 0;
}

