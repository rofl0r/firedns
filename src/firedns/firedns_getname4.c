#include "firedns_internal.h"

int firedns_getname4(const struct in_addr * restrict const ip) { 
	char query[512];
	struct s_header h;
	struct s_connection * restrict s;
	unsigned char *c;
	int l;
	firedns_init();
	c = (unsigned char *)&ip->s_addr;
	sprintf(query,"%d.%d.%d.%d.in-addr.arpa",c[3],c[2],c[1],c[0]);
	l = firedns_build_query_payload(query,FDNS_QRY_PTR,1,(unsigned char *)&h.payload);
	if (l == -1)
		return -1;
	s = firedns_add_query(&h);
	if (s == NULL)
		return -1;
	s->class = 1;
	s->type = FDNS_QRY_PTR;
	if (firedns_send_requests(&h,s,l) == -1)
		return -1;
	return s->fd;
}

