#include "firestring_internal.h"

long firestring_estr_estrstr(const struct firestring_estr_t * restrict const haystack, const struct firestring_estr_t * restrict const needle, const long start) {
	long i,l;
	l = haystack->l - needle->l;
	for (i = start; i <= l; i++) {
		if (memcmp(&haystack->s[i],needle->s,needle->l) == 0)
				return i;
	}
	return -1;
}

