#include "firedns_internal.h"

void firedns_fill_rr(struct s_rr_middle * restrict const rr, const unsigned char * const restrict input) {
	rr->type = input[0] * 256 + input[1];
	rr->dclass = input[2] * 256 + input[3];
	rr->ttl = input[4] * 16777216 + input[5] * 65536 + input[6] * 256 + input[7];
	rr->rdlength = input[8] * 256 + input[9];
}

