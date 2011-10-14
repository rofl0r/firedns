#include "firedns_internal.h"

char *firedns_resolvename4_i(firedns_state* self, const struct in_addr* ip, char *(* const result)(firedns_state*, int)) {
	int fd;
	int t,i;
	char* ret;
	fd_set s;
	struct timeval tv;
	for (t = 0; t < FIREDNS_TRIES; t++) {
		fd = firedns_getname4(self, ip);
		if (fd == -1)
			return NULL;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&s);
		FD_SET(fd,&s);
		i = select(fd + 1,&s,NULL,NULL,&tv);
		ret = result(self, fd);
		if (ret != NULL || i != 0)
			return ret;
	}
	return NULL;
}

