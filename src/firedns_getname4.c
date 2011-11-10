#include "firedns_internal.h"
#include "../../lib/include/strlib.h"

int firedns_getname4(firedns_state* self, const struct in_addr* ip) { 
	char query[512];
	struct s_header h;
	struct s_connection* s;
	unsigned char *c;
	int l;
	c = (unsigned char *)&ip->s_addr;
	ulz_snprintf(query, sizeof(query), "%d.%d.%d.%d.in-addr.arpa",c[3],c[2],c[1],c[0]);
	l = firedns_build_query_payload(query,FDNS_QRY_PTR,1,(unsigned char *)&h.payload);
	if (l == -1)
		return -1;
	s = firedns_add_query(self, &h);
	if (s == NULL)
		return -1;
	s->dclass = 1;
	s->type = FDNS_QRY_PTR;
	if (firedns_send_requests(self, &h, s, l) == -1)
		return -1;
	return s->fd;
}

