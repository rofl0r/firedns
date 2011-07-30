#include "firedns_internal.h"

void firedns_close(int fd) { 
	if (fd == lastcreate) {
		wantclose = 1;
		return;
	}
	close(fd);
	return;
}

