#include "firestring_internal.h"

void firestring_estr_chug(struct firestring_estr_t * const s) { 
	while (s->l > 0 && isspace(s->s[0])) {
		s->s++;
		s->l--;
		s->a--;
	}
}

