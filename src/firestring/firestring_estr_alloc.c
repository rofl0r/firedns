#include "firestring_internal.h"

void firestring_estr_alloc(struct firestring_estr_t * const f, const long a) {
	if (a + 1 > UINT_MAX) {
		errno = EINVAL;
		error_handler();
	}
	f->s = firestring_malloc(a + 1);
	f->a = a;
	f->l = 0;
}

