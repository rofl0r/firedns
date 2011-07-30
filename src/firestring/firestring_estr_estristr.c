#include "firestring_internal.h"

long firestring_estr_estristr(const struct firestring_estr_t * restrict const haystack, const struct firestring_estr_t * restrict const needle, const long start) {
	long i,j,l;
	l = haystack->l - needle->l;
	for (i = start; i <= l; i++) {
		for (j = 0; j < needle->l; j++)
			if (tolower(haystack->s[i+j]) != tolower(needle->s[j]))
					break;
		if (j == needle->l)
			return i;
	}
	return -1;
}

