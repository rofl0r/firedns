#include "firedns_internal.h"

struct firedns_ip4list *firedns_resolveip4list_r(const char * const name) { 
	return firedns_resolveip4list_i(name,firedns_getresult_r);
}

