#include "firedns_internal.h"

char *firedns_ntoa6_r(const struct in6_addr * restrict ip) {
	char * restrict result;
	result = firestring_malloc(256);
	return firedns_ntoa6_s(ip,result);
}

