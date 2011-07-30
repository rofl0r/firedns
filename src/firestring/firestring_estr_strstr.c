#include "firestring_internal.h"

long firestring_estr_strstr(const struct firestring_estr_t * restrict const f, const char * restrict s, const long start) {
	long i;
	long l;
	l = strlen(s);
	for (i = start; i <= f->l - l; i++)
		if (memcmp(&f->s[i],s,l) == 0)
			return i;
	return -1;
}

