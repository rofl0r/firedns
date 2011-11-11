#include "firedns_internal.h"
#include <unistd.h>

void firedns_close(firedns_state* self, int fd) { 
	if (fd == self->lastcreate) {
		self->wantclose = 1;
		return;
	}
	close(fd);
	return;
}

