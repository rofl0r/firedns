#include "firedns_internal.h"

int firedns_getip4(const char * restrict const name) { 
	struct s_header h;
	struct s_connection * restrict s;
	int l;
	firedns_init();
	l = firedns_build_query_payload(name,FDNS_QRY_A,1,(unsigned char *)&h.payload);
	if (l == -1)
		return -1;
	s = firedns_add_query(&h);
	if (s == NULL)
		return -1;
	s->class = 1;
	s->type = FDNS_QRY_A;
	if (firedns_send_requests(&h,s,l) == -1)
		return -1;
	return s->fd;
}
