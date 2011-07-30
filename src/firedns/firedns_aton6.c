#include "firedns_internal.h"

struct in6_addr *firedns_aton6(const char * const ipstring) {
	static struct in6_addr ip;
	return firedns_aton6_s(ipstring,&ip);
}

