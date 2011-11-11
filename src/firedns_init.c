#include "firedns_internal.h"
#include "../../lib/include/fileparser.h"

void firedns_init(firedns_state* self) { 
	fileparser ff, *f = &ff;
	size_t i;
	struct in_addr addr4;
#ifdef HAVE_IPV6
	struct in6_addr addr6;
#endif
	memset(self, 0, sizeof(firedns_state));

	self->lastcreate = -1;
	
	srand((unsigned int) time(NULL));
	
	if(fileparser_open(f, FDNS_CONFIG_FBCK))
		return;
	while(!fileparser_readline(f)) {
		if (strncmp(f->buf, "nameserver", 10) == 0) {
			i = 10;
			while (i < f->len && (f->buf[i] == ' ' || f->buf[i] == '\t'))
				i++;
#ifdef HAVE_IPV6
			if (self->i6 < FDNS_MAX) {
				if (firedns_aton6(&f->buf[i],&addr6) != NULL) {
					memcpy(&self->servers6[(self->i6)++], &addr6, sizeof(struct in6_addr));
					continue;
				}
			}
#endif
			if (self->i4 < FDNS_MAX) {
				if (firedns_aton4(&f->buf[i],&addr4) != NULL)
					memcpy(&self->servers4[(self->i4)++], &addr4, sizeof(struct in_addr));
			}
		}
	}

	fileparser_close(f);
}

