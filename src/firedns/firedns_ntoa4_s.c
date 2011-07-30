#include "firedns_internal.h"

char *firedns_ntoa4_s(const struct in_addr * restrict const ip, char * restrict const result) { 
	unsigned char *m;
	m = (unsigned char *)&ip->s_addr;
	sprintf(result,"%d.%d.%d.%d",m[0],m[1],m[2],m[3]);
	return result;
}

