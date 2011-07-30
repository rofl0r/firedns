#include "firestring_internal.h"

long firestring_estr_strchr(const struct firestring_estr_t * const f, const char c, const long start) {
	long i;
	for (i = start; i < f->l; i++)
		if (f->s[i] == c)
			return i;
	return -1;
}

