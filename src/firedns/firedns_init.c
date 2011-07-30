#include "firedns_internal.h"
#include "../../../lib/include/fileparser.h"

void firedns_init() { 
	fileparser ff, *f = &ff;
	size_t i;
	struct in_addr addr4;
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
	if(fileparser_open(f, FDNS_CONFIG_FBCK))
		return;
	while(!fileparser_readline(f)) {
		if (strncmp(f->buf,"nameserver",10) == 0) {
			i = 10;
			while (i < f->len && (f->buf[i] == ' ' || f->buf[i] == '\t'))
				i++;
#ifdef HAVE_IPV6
			if (i6 < FDNS_MAX) {
				if (firedns_aton6_s(&f->buf[i],&addr6) != NULL) {
					memcpy(&servers6[i6++],&addr6,sizeof(struct in6_addr));
					continue;
				}
			}
#endif
			if (i4 < FDNS_MAX) {
				if (firedns_aton4_s(&f->buf[i],&addr4) != NULL)
					memcpy(&servers4[i4++],&addr4,sizeof(struct in_addr));
			}
		}
	}

	fileparser_close(f);
}

