#include "firestring_internal.h"

int firestring_estr_estarts(const struct firestring_estr_t * restrict const f, const struct firestring_estr_t * restrict const s) {
	long i;
	if (f->l < s->l)
		return 1;
	for (i = 0; i < s->l; i++)
		if (tolower(f->s[i]) != tolower(s->s[i]))
			return 1;
	return 0;
}

