#include "firedns_internal.h"

struct in_addr *firedns_aton4(const char * const ipstring) { 
	static struct in_addr ip;
	return firedns_aton4_s(ipstring,&ip);
}

