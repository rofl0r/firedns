#include "../../lib/include/strlib.h"
#include "firedns_internal.h"

char *firedns_ntoa4_s(const struct in_addr* ip, char* result) { 
	unsigned char *m;
	m = (unsigned char *)&ip->s_addr;
	ulz_snprintf(result, 16, "%d.%d.%d.%d",m[0],m[1],m[2],m[3]);
	return result;
}

