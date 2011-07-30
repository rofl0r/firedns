#include "firedns_internal.h"

struct firedns_txtlist *firedns_resolvetxtlist(const char * const name) { 
	return firedns_resolvetxtlist_i(name,firedns_getresult);
}

