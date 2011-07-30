#include "firedns_internal.h"

struct firedns_ip6list *firedns_resolveip6list(const char * const name) { 
	return firedns_resolveip6list_i(name,firedns_getresult);
}

