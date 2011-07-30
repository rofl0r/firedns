#include "firestring_internal.h"

int firestring_estr_eends(const struct firestring_estr_t * restrict const f, const struct firestring_estr_t * restrict const s) {
	long i,t;
	if (f->l < s->l)
		return 1;
	t = f->l - s->l;
	for (i = f->l - 1; i >= t; i--)
		if (tolower(f->s[i]) != tolower(s->s[i - t]))
			return 1;
	return 0;
}

