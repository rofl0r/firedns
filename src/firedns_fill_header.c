#include "firedns_internal.h"

void firedns_fill_header(struct s_header * const restrict header, const unsigned char * const restrict input, const int l) {
	header->id[0] = input[0];
	header->id[1] = input[1];
	header->flags1 = input[2];
	header->flags2 = input[3];
	header->qdcount = input[4] * 256 + input[5];
	header->ancount = input[6] * 256 + input[7];
	header->nscount = input[8] * 256 + input[9];
	header->arcount = input[10] * 256 + input[11];
	memcpy(header->payload,&input[12],l);
}

