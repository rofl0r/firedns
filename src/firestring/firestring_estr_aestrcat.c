#include "firestring_internal.h"

int firestring_estr_aestrcat(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	firestring_estr_expand(t,t->l + (f->l - start));
	return firestring_estr_estrcat(t,f,start);
}

