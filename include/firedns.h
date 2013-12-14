/*

firedns.h - firedns library declarations
Copyright (C) 2002 Ian Gulliver

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef _FIREDNS_H
#define _FIREDNS_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef _FIREDNS_C
extern const char firedns_version[];
extern const int firedns_mx_port[];
extern const char *firedns_mx_name[];
#endif

#define FDNS_MAX              8

struct s_connection {
	struct s_connection *next;
	unsigned char id[2];
	signed char v6;
	signed char want_list;
	uint16_t dclass;
	uint16_t type;
	int fd;
};

typedef struct firedns_state {
	char resultbuf[1024];
	struct s_connection *connection_head;
	struct s_connection connections[FDNS_MAX];
	char conn_used[FDNS_MAX];
	struct in_addr servers4[FDNS_MAX];
	int wantclose;
	int lastcreate;
	int i4;
#ifdef HAVE_IPV6
	int i6;
	struct in6_addr servers6[FDNS_MAX];
#endif
} firedns_state;

#define FIREDNS_MX_SMTP 0
#define FIREDNS_MX_QMTP 1

struct firedns_ip4list {
	struct in_addr ip;
	struct firedns_ip4list *next;
};

struct firedns_ip6list {
	struct in6_addr ip;
	struct firedns_ip6list *next;
};

struct firedns_mxlist {
	unsigned int priority;
	unsigned int protocol;
	struct firedns_ip4list *ip4list;
	struct firedns_ip6list *ip6list;
	char *cname;
	char *name;
	struct firedns_mxlist *next;
};

struct firedns_txtlist {
	char *txt;
	struct firedns_txtlist *next;
};

/******* ATTENTION: 
 * 
 *  after setting up your firedns_state using firedns_init,
 *  you have to either call firedns_add_servers_from_resolv_conf() or
 *  add single dns servers using firedns_add_server() 
 *  before you call any other firedns function! 
 * 
 ******************/


/* state-free helper functions */
struct in_addr *firedns_aton4(const char* ipstring, struct in_addr* ip);
struct in6_addr *firedns_aton6(const char* ipstring, struct in6_addr* ip);
char *firedns_ntoa4(const struct in_addr* ip, char* result);
char *firedns_ntoa6(const struct in6_addr* ip, char* result);

/* initialize the firedns struct */
void firedns_init(firedns_state* self);

/* add a name server to the internal list.
 * returns 0 on success, else the number of fails (2 possible, one for ipv6, one for ipv4) */
int firedns_add_server(firedns_state* self, char* server);
/* reads /etc/resolv.conf and calls firedns_add_server for each entry */
int firedns_add_servers_from_resolv_conf(firedns_state* self);

struct s_connection* firedns_getconn(firedns_state* self);
void firedns_freeconn(firedns_state* self, struct s_connection* conn);

/* non-blocking functions */
int firedns_getip4(firedns_state* self, const char* name);
int firedns_getip4list(firedns_state* self, const char* name);
int firedns_getip6(firedns_state* self, const char* name);
int firedns_getip6list(firedns_state* self, const char* name);
int firedns_gettxt(firedns_state* self, const char* name);
int firedns_gettxtlist(firedns_state* self, const char* name);
int firedns_getmx(firedns_state* self, const char* name);
int firedns_getmxlist(firedns_state* self, const char* name);
int firedns_getname4(firedns_state* self, const struct in_addr* ip);
int firedns_getname6(firedns_state* self, const struct in6_addr* ip);
int firedns_getcname(firedns_state* self, const char* name);
char *firedns_getresult(firedns_state* self, const int fd);

/* low-timeout blocking functions */
struct in_addr *firedns_resolveip4(firedns_state* self, const char* name);
struct firedns_ip4list *firedns_resolveip4list(firedns_state* self, const char* name);
struct in6_addr *firedns_resolveip6(firedns_state* self, const char* name);
struct firedns_ip6list *firedns_resolveip6list(firedns_state* self, const char* name);
char *firedns_resolvetxt(firedns_state* self, const char* name);
struct firedns_txtlist *firedns_resolvetxtlist(firedns_state* self, const char* name);
char *firedns_resolvemx(firedns_state* self, const char* name);
struct firedns_mxlist *firedns_resolvemxlist(firedns_state* self, const char* name);
//struct firedns_mxlist *firedns_resolvemxalist(firedns_state* self, const char* name);
char *firedns_resolvename4(firedns_state* self, const struct in_addr* ip);
char *firedns_resolvename6(firedns_state* self, const struct in6_addr* ip);
char *firedns_resolvecname(firedns_state* self, const char* name);


/* misc functions */
void firedns_free_mxalist(struct firedns_mxlist *list);

#endif
//RcB: DEP "../src/*.c"

