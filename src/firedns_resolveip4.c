#include <sys/time.h>
#include "firedns_internal.h"
#ifdef USE_LIBULZ
#include <ulz/strlib.h>
#endif

struct in_addr* firedns_resolveip4(firedns_state* self, const char * name) {
	int fd;
	int t,i;
	struct in_addr* ret;
	fd_set s;
	struct timeval tv;

#ifdef USE_LIBULZ
	if(isnumericipv4(name)) {
		ipv4fromstring((char*)name, (unsigned char*) self->resultbuf);
		return (struct in_addr*) self->resultbuf;
	}
#endif
	for (t = 0; t < FIREDNS_TRIES; t++) {
		fd = firedns_getip4(self, name);
		if (fd == -1)
			return 0;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&s);
		FD_SET(fd,&s);
		i = select(fd + 1, &s, 0, 0, &tv);
		ret = (struct in_addr *) firedns_getresult(self, fd);
		if (ret || i != 0)
			return ret;
	}
	return 0;
}

