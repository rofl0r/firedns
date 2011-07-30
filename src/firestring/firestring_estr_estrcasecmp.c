#include "firestring_internal.h"

int firestring_estr_estrcasecmp(const struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	long i;
	if (f->l != t->l - start)
		return 1;
	for (i = 0; i < f->l; i++)
		if (tolower(f->s[i]) != tolower(t->s[i + start]))
			return 1;
	return 0;
}

