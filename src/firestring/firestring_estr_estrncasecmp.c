#include "firestring_internal.h"

int firestring_estr_estrncasecmp(const struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long length, const long start) {
	long i;
	for (i = 0; i < length; i++)
		if (tolower(f->s[i]) != tolower(t->s[i + start]))
			return 1;
	return 0;
}

