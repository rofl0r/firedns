#include "firestring_internal.h"

void firestring_estr_chomp(struct firestring_estr_t * const s) { 
	while (s->l > 0 && isspace(s->s[s->l - 1]))
		s->l--;
}

