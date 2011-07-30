#include "firestring_internal.h"

void firestring_estr_ip_chug(struct firestring_estr_t * const s) { 
	long start = 0;
	while (start < s->l && isspace(s->s[start]))
		start++;
	memmove(s->s,&s->s[start],s->l - start);
	s->l -= start;
}

