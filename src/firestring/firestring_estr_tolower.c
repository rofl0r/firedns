#include "firestring_internal.h"

int firestring_estr_tolower(struct firestring_estr_t * const dest, const struct firestring_estr_t * const source, const long start) {
	long i;
	if (source->l - start > dest->a)
		return 1;
	for (i = start; i < source->l; i++)
		dest->s[i - start] = tolower(source->s[i]);
	return 0;
}

