#include "firestring_internal.h"

void firestring_estr_free(struct firestring_estr_t * const f) {
	if (f->s != NULL)
		free(f->s);
	f->a = 0;
	f->l = 0;
	f->s = NULL;
}

