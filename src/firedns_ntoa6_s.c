#include "../../lib/include/strlib.h"
#include "firedns_internal.h"

char *firedns_ntoa6_s(const struct in6_addr * restrict const ip, char * restrict const result) {
	char *c;
	ulz_snprintf(result, 48, "%x:%x:%x:%x:%x:%x:%x:%x",
			ntohs(*((unsigned short *)&ip->s6_addr[0])),
			ntohs(*((unsigned short *)&ip->s6_addr[2])),
			ntohs(*((unsigned short *)&ip->s6_addr[4])),
			ntohs(*((unsigned short *)&ip->s6_addr[6])),
			ntohs(*((unsigned short *)&ip->s6_addr[8])),
			ntohs(*((unsigned short *)&ip->s6_addr[10])),
			ntohs(*((unsigned short *)&ip->s6_addr[12])),
			ntohs(*((unsigned short *)&ip->s6_addr[14])));
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

