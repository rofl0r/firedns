#include "firestring_internal.h"

long firestring_estr_stristr(const struct firestring_estr_t * restrict const f, const char * restrict s, const long start) {
	long i;
	long j;
	long l;
	l = strlen(s);
	for (i = start; i <= f->l - l; i++) {
		for (j = 0; j < l; j++)
			if (tolower(f->s[i+j]) != tolower(s[j]))
					break;
		if (j == l)
			return i;
	}
	return -1;
}

