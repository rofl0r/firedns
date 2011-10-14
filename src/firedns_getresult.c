#include "firedns_internal.h"

char *firedns_getresult(firedns_state* self, const int fd) { 
	return firedns_getresult_s(self, fd);
}

