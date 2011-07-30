#include "firestring_internal.h"

void firestring_estr_ip_trim(struct firestring_estr_t * const s) {
	firestring_estr_ip_chug(s);
	firestring_estr_chomp(s);
}

