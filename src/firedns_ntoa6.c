#include "firedns_internal.h"

char *firedns_ntoa6(const struct in6_addr *ip) {
	static char result[256];
	return firedns_ntoa6_s(ip,result);
}

