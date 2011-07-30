#include "firedns_internal.h"

int firedns_dnsbl_lookup_txt(const struct in_addr * restrict const ip, const char * restrict const name) { 
	char hostname[256];
	firestring_snprintf(hostname,256,"%u.%u.%u.%u.%s",(unsigned int) ((unsigned char *)&ip->s_addr)[3],(unsigned int) ((unsigned char *)&ip->s_addr)[2],(unsigned int) ((unsigned char *)&ip->s_addr)[1],(unsigned int) ((unsigned char *)&ip->s_addr)[0],name);
	return firedns_gettxt(hostname);
}

