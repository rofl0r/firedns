#include "firedns_internal.h"
#include <string.h>

int firedns_send_requests(firedns_state* self, const struct s_header * restrict const h, const struct s_connection * restrict const s, const int l) { 
	int i;
	struct sockaddr_in addr4;
	unsigned char payload[sizeof(struct s_header)];
#ifdef HAVE_IPV6
	struct sockaddr_in6 addr6;
#endif
	firedns_empty_header(payload,h,l);
#ifdef HAVE_IPV6
	
	if (i6 > 0 && s->v6 == 1) {
		for (i = 0; i < self->i6; i++) {
			memset(&addr6,0,sizeof(addr6));
			memcpy(&addr6.sin6_addr,&servers6[i],sizeof(addr6.sin6_addr));
			addr6.sin6_family = AF_INET6;
			addr6.sin6_port = htons(FDNS_PORT);
			sendto(s->fd, payload, l + 12, 0, (struct sockaddr *) &addr6, sizeof(addr6));
		}
	}
#endif
	for (i = 0; i < self->i4; i++) {
#ifdef HAVE_IPV6
		
		if (s->v6 == 1) {
			memset(&addr6,0,sizeof(addr6));
			memcpy(addr6.sin6_addr.s6_addr,"\0\0\0\0\0\0\0\0\0\0\xff\xff",12);
			memcpy(&addr6.sin6_addr.s6_addr[12],&servers4[i].s_addr,4);
			addr6.sin6_family = AF_INET6;
			addr6.sin6_port = htons(FDNS_PORT);
			sendto(s->fd, payload, l + 12, 0, (struct sockaddr *) &addr6, sizeof(addr6));
			continue;
		}
#endif
		
		memset(&addr4,0,sizeof(addr4));
		memcpy(&addr4.sin_addr,&self->servers4[i],sizeof(addr4.sin_addr));
		addr4.sin_family = AF_INET;
		addr4.sin_port = htons(FDNS_PORT);
		sendto(s->fd, payload, l + 12, 0, (struct sockaddr *) &addr4, sizeof(addr4));
	}
	return 0;
}

