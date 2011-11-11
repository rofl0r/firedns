#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "firedns_internal.h"

void firedns_init(firedns_state* self) { 
	memset(self, 0, sizeof(firedns_state));
	self->lastcreate = -1;
	srand((unsigned int) time(NULL));
}

