#include "firestring_internal.h"

int firestring_estr_replace(struct firestring_estr_t * const dest, const struct firestring_estr_t * const source, const struct firestring_estr_t * const to, const struct firestring_estr_t * const from) {
	int i = 0, oldi = 0;
	struct firestring_estr_t tempe;
	dest->l = 0;
	while ((i = firestring_estr_estrstr(source,from,i)) != -1) {
		tempe.s = &source->s[oldi];
		tempe.l = tempe.a = i - oldi;
		if (firestring_estr_estrcat(dest,&tempe,0) != 0)
			return 1;
		if (firestring_estr_estrcat(dest,to,0) != 0)
			return 1;
		i = oldi = i + from->l;
	}
	tempe.s = &source->s[oldi];
	tempe.l = tempe.a = source->l - oldi;
	if (firestring_estr_estrcat(dest,&tempe,0) != 0)
		return 1;
	return 0;
}

