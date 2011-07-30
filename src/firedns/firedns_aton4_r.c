#include "firedns_internal.h"

struct in_addr *firedns_aton4_r(const char * restrict const ipstring) { 
	struct in_addr * restrict ip;
	ip = firestring_malloc(sizeof(struct in_addr));
	if(firedns_aton4_s(ipstring,ip) == NULL) {
		free(ip);
		return NULL;
	}
	return ip;
}

