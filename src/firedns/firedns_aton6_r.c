#include "firedns_internal.h"

struct in6_addr *firedns_aton6_r(const char * restrict const ipstring) {
	struct in6_addr * restrict ip;
	ip = firestring_malloc(sizeof(struct in6_addr));
	if(firedns_aton6_s(ipstring,ip) == NULL) {
		free(ip);
		return NULL;
	}
	return ip;
}

