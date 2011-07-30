#include "firedns_internal.h"

char *firedns_ntoa4_r(const struct in_addr * restrict const ip) { 
	char * restrict result;
	result = firestring_malloc(256);
	return firedns_ntoa4_s(ip,result);
}

