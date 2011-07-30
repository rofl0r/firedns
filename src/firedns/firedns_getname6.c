#include "firedns_internal.h"

int firedns_getname6(const struct in6_addr * restrict const ip) {
	char query[512];
	struct s_header h;
	struct s_connection * restrict s;
	int l;
	firedns_init();
	sprintf(query,"%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.%0x.ip6.int",
			ip->s6_addr[15] & 0x0f,
			(ip->s6_addr[15] & 0xf0) >> 4,
			ip->s6_addr[14] & 0x0f,
			(ip->s6_addr[14] & 0xf0) >> 4,
			ip->s6_addr[13] & 0x0f,
			(ip->s6_addr[13] & 0xf0) >> 4,
			ip->s6_addr[12] & 0x0f,
			(ip->s6_addr[12] & 0xf0) >> 4,
			ip->s6_addr[11] & 0x0f,
			(ip->s6_addr[11] & 0xf0) >> 4,
			ip->s6_addr[10] & 0x0f,
			(ip->s6_addr[10] & 0xf0) >> 4,
			ip->s6_addr[9] & 0x0f,
			(ip->s6_addr[9] & 0xf0) >> 4,
			ip->s6_addr[8] & 0x0f,
			(ip->s6_addr[8] & 0xf0) >> 4,
			ip->s6_addr[7] & 0x0f,
			(ip->s6_addr[7] & 0xf0) >> 4,
			ip->s6_addr[6] & 0x0f,
			(ip->s6_addr[6] & 0xf0) >> 4,
			ip->s6_addr[5] & 0x0f,
			(ip->s6_addr[5] & 0xf0) >> 4,
			ip->s6_addr[4] & 0x0f,
			(ip->s6_addr[4] & 0xf0) >> 4,
			ip->s6_addr[3] & 0x0f,
			(ip->s6_addr[3] & 0xf0) >> 4,
			ip->s6_addr[2] & 0x0f,
			(ip->s6_addr[2] & 0xf0) >> 4,
			ip->s6_addr[1] & 0x0f,
			(ip->s6_addr[1] & 0xf0) >> 4,
			ip->s6_addr[0] & 0x0f,
			(ip->s6_addr[0] & 0xf0) >> 4
			);
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

