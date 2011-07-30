#include "firedns_internal.h"

struct in6_addr *firedns_resolveip6_i(const char * restrict const name, char *(* const result)(int)) {
	int fd;
	int t,i;
	struct in6_addr * restrict ret;
	fd_set s;
	struct timeval tv;
	for (t = 0; t < FIREDNS_TRIES; t++) {
		fd = firedns_getip6(name);
		if (fd == -1)
			return NULL;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&s);
		FD_SET(fd,&s);
		i = select(fd + 1,&s,NULL,NULL,&tv);
		ret = (struct in6_addr *) result(fd);
		if (ret != NULL || i != 0)
			return ret;
	}
	return NULL;
}

