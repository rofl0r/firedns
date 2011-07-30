#include "firestring_internal.h"

int firestring_estr_munch(struct firestring_estr_t * const t, const long length) {
	if (length > t->l)
		return 1;
	t->l -= length;
	memmove(t->s,&t->s[length],t->l);
	return 0;
}

