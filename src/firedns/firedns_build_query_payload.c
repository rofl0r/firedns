#include "firedns_internal.h"

int firedns_build_query_payload(const char * const name, const unsigned short rr, const unsigned short class, unsigned char * const payload) { 
	short payloadpos;
	const char * tempchr, * tempchr2;
	unsigned short l;
	payloadpos = 0;
	tempchr2 = name;
	
	while ((tempchr = strchr(tempchr2,'.')) != NULL) {
		l = tempchr - tempchr2;
		if (payloadpos + l + 1 > 507)
			return -1;
		payload[payloadpos++] = l;
		memcpy(&payload[payloadpos],tempchr2,l);
		payloadpos += l;
		tempchr2 = &tempchr[1];
	}
	l = strlen(tempchr2);
	if (l) {
		if (payloadpos + l + 2 > 507)
			return -1;
		payload[payloadpos++] = l;
		memcpy(&payload[payloadpos],tempchr2,l);
		payloadpos += l;
		payload[payloadpos++] = '\0';
	}
	if (payloadpos > 508)
		return -1;
	l = htons(rr);
	memcpy(&payload[payloadpos],&l,2);
	l = htons(class);
	memcpy(&payload[payloadpos + 2],&l,2);
	return payloadpos + 4;
}

