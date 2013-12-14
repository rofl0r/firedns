#undef _GNU_SOURCE
#define _GNU_SOURCE
#include "firedns_internal.h"
#include <string.h>
#include <stdio.h>
#ifdef USE_LIBULZ
#include <ulz/stdio-repl.h>
#endif

char *firedns_ntoa6(const struct in6_addr * restrict const ip, char * restrict const result) {
	char *c;
	snprintf(result, 48, "%x:%x:%x:%x:%x:%x:%x:%x",
			ntohs(ip->s6_addr16[0]),
			ntohs(ip->s6_addr16[1]),
			ntohs(ip->s6_addr16[2]),
			ntohs(ip->s6_addr16[3]),
			ntohs(ip->s6_addr16[4]),
			ntohs(ip->s6_addr16[5]),
			ntohs(ip->s6_addr16[6]),
			ntohs(ip->s6_addr16[7]));
	c = strstr(result,":0:");
	if (c != NULL) {
		memmove(c+1,c+2,strlen(c+2) + 1);
		c += 2;
		while (memcmp(c,"0:",2) == 0)
			memmove(c,c+2,strlen(c+2) + 1);
		if (memcmp(c,"0",2) == 0)
			*c = '\0';
		if (memcmp(result,"0::",3) == 0)
			memmove(result,result + 1, strlen(result + 1) + 1);
	}
	return result;
}

