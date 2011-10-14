#include "firedns_internal.h"

int firedns_getip4list(firedns_state* self, const char* name) { 
	struct s_header h;
	struct s_connection* s;
	int l;
	l = firedns_build_query_payload(name, FDNS_QRY_A, 1, (unsigned char*) &h.payload);
	if (l == -1)
		return -1;
	s = firedns_add_query(self, &h);
	if (s == NULL)
		return -1;
	s->class = 1;
	s->type = FDNS_QRY_A;
	s->want_list = 1;
	if (firedns_send_requests(self, &h, s, l) == -1)
		return -1;
	return s->fd;
}

