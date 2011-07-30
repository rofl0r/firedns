#include "firedns_internal.h"

void firedns_init() { 
	FILE *f;
	int i;
	struct in_addr addr4;
	char buf[1024];
#ifdef HAVE_IPV6
	struct in6_addr addr6;
#endif
	if (initdone == 1)
		return;
#ifdef HAVE_IPV6
	i6 = 0;
#endif
	i4 = 0;
	initdone = 1;
	srand((unsigned int) time(NULL));
	memset(servers4,'\0',sizeof(struct in_addr) * FDNS_MAX);
#ifdef HAVE_IPV6
	memset(servers6,'\0',sizeof(struct in6_addr) * FDNS_MAX);
#endif
	
	f = fopen(FDNS_CONFIG_PREF,"r");
	if (f == NULL) {
		f = fopen(FDNS_CONFIG_FBCK,"r");
		if (f == NULL)
			return;
		while (fgets(buf,1024,f) != NULL) {
			if (strncmp(buf,"nameserver",10) == 0) {
				i = 10;
				while (buf[i] == ' ' || buf[i] == '\t')
					i++;
#ifdef HAVE_IPV6
				
				if (i6 < FDNS_MAX) {
					if (firedns_aton6_s(&buf[i],&addr6) != NULL) {
						memcpy(&servers6[i6++],&addr6,sizeof(struct in6_addr));
						continue;
					}
				}
#endif
				if (i4 < FDNS_MAX) {
					if (firedns_aton4_s(&buf[i],&addr4) != NULL)
						memcpy(&servers4[i4++],&addr4,sizeof(struct in_addr));
				}
			}
		}
	} else {
		while (fgets(buf,1024,f) != NULL) {
#ifdef HAVE_IPV6
			if (i6 < FDNS_MAX) {
				if (firedns_aton6_s(buf,&addr6) != NULL) {
					memcpy(&servers6[i6++],&addr6,sizeof(struct in6_addr));
					continue;
				}
			}
#endif
			if (i4 < FDNS_MAX) {
				if (firedns_aton4_s(buf,&addr4) != NULL)
					memcpy(&servers4[i4++],&addr4,sizeof(struct in_addr));
			}
		}
	}
	fclose(f);
}

