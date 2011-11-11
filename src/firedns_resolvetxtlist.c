#include "firedns_internal.h"
#include <unistd.h>
#include <sys/time.h>

struct firedns_txtlist *firedns_resolvetxtlist(firedns_state* self, const char* name) { 
	int fd;
	int t,i;
	struct firedns_txtlist* ret;
	fd_set s;
	struct timeval tv;
	for (t = 0; t < FIREDNS_TRIES; t++) {
		fd = firedns_gettxtlist(self, name);
		if (fd == -1)
			return NULL;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&s);
		FD_SET(fd,&s);
		i = select(fd + 1,&s,NULL,NULL,&tv);
		ret = (struct firedns_txtlist *) firedns_getresult(self, fd);
		if (ret != NULL || i != 0)
			return ret;
	}
	return NULL;
}

