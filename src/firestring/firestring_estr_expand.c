#include "firestring_internal.h"

void firestring_estr_expand(struct firestring_estr_t * const f, const long a) {
	if (f->a >= a)
		return;
	if (a + 1 > UINT_MAX) {
		errno = EINVAL;
		error_handler();
	}
	f->s = firestring_realloc(f->s, a + 1);
	f->a = a;
}

