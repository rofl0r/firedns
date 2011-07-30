#include "firedns_internal.h"

void firedns_empty_header(unsigned char * const restrict output, const struct s_header * const restrict header, const int l) {
	output[0] = header->id[0];
	output[1] = header->id[1];
	output[2] = header->flags1;
	output[3] = header->flags2;
	output[4] = header->qdcount / 256;
	output[5] = header->qdcount % 256;
	output[6] = header->ancount / 256;
	output[7] = header->ancount % 256;
	output[8] = header->nscount / 256;
	output[9] = header->nscount % 256;
	output[10] = header->arcount / 256;
	output[11] = header->arcount % 256;
	memcpy(&output[12],header->payload,l);
}

