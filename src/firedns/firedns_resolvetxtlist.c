#include "firedns_internal.h"

struct firedns_txtlist *firedns_resolvetxtlist(firedns_state* self, const char* name) { 
	return firedns_resolvetxtlist_i(self, name,firedns_getresult);
}

