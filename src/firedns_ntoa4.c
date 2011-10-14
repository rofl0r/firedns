#include "firedns_internal.h"

char *firedns_ntoa4(const struct in_addr * const ip) { 
	static char result[256];
	return firedns_ntoa4_s(ip,result);
}

