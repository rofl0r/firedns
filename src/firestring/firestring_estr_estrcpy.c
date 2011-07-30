#include "firestring_internal.h"

int firestring_estr_estrcpy(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	if (f->l - start > t->a)
		return 1;
	t->l = f->l - start;
	memcpy(t->s,&f->s[start],t->l);
	return 0;
}

