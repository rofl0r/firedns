#include "firedns_internal.h"
#include "../../lib/include/strlib.h"

struct in_addr* firedns_resolveip4_i(firedns_state* self, const char * name, char *(* const result)(firedns_state*, int)) { 
	int fd;
	int t,i;
	struct in_addr* ret;
	fd_set s;
	struct timeval tv;
	
	if(isnumericipv4(name)) {
		ipv4fromstring((char*)name, (unsigned char*) self->resultbuf);
		return (struct in_addr*) self->resultbuf;
	}
	
	for (t = 0; t < FIREDNS_TRIES; t++) {
		fd = firedns_getip4(self, name);
		if (fd == -1)
			return NULL;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&s);
		FD_SET(fd,&s);
		i = select(fd + 1,&s,NULL,NULL,&tv);
		ret = (struct in_addr *) result(self, fd);
		if (ret != NULL || i != 0)
			return ret;
	}
	return NULL;
}

