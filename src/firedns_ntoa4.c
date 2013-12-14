#include "firedns_internal.h"
#include <stdio.h>
#ifdef USE_LIBULZ
#include <ulz/stdio-repl.h>
#endif

char *firedns_ntoa4(const struct in_addr* ip, char* result) {
	unsigned char *m;
	m = (unsigned char *)&ip->s_addr;
	snprintf(result, 16, "%d.%d.%d.%d",m[0],m[1],m[2],m[3]);
	return result;
}

