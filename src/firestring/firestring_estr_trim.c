#include "firestring_internal.h"

void firestring_estr_trim(struct firestring_estr_t * const s) {
	firestring_estr_chug(s);
	firestring_estr_chomp(s);
}

