#include "firestring_internal.h"

int firestring_estr_estrcat(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	if (f->l - start + t->l > t->a)
		return 1;
	memcpy(&t->s[t->l],&f->s[start],f->l - start);
	t->l += f->l - start;
	return 0;
}

