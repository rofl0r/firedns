#include "firedns_internal.h"
#include <unistd.h>
#include <sys/time.h>

char *firedns_resolvetxt(firedns_state* self, const char* name) {
	int fd;
	int t,i;
	char * restrict ret;
	fd_set s;
	struct timeval tv;
	for (t = 0; t < FIREDNS_TRIES; t++) {
		fd = firedns_gettxt(self, name);
		if (fd == -1)
			return NULL;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&s);
		FD_SET(fd,&s);
		i = select(fd + 1,&s,NULL,NULL,&tv);
		ret = firedns_getresult(self, fd);
		if (ret != NULL || i != 0)
			return ret;
	}
	return NULL;
}

