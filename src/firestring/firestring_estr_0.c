#include "firestring_internal.h"

void firestring_estr_0(struct firestring_estr_t * const f) {
	if (f->s != NULL)
		f->s[f->l] = '\0';
}

