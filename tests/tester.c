#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include "../include/firedns.h"

// TODO: this test is full of old code. need to be converted to use firedns_init and firedns_state.
// also the whole do_ stuff seems to duplicate the library code.
// it definitely will not compile currently.

void do_firedns_aton4(firedns_state *state, char * string) {
	(void) state;
	struct in_addr *inaddr4;
	struct in_addr buf4;
	fprintf(stderr,"taking firedns_aton4(%s): ",string);
	inaddr4 = firedns_aton4(string, &buf4);
	if (inaddr4 == NULL)
		fprintf(stderr,"got NULL\n");
	else {
		char result[256];
		firedns_ntoa4(inaddr4, result);
		fprintf(stderr,"%d.%d.%d.%d/%s\n",((unsigned char *)&inaddr4->s_addr)[0],
				((unsigned char *)&inaddr4->s_addr)[1],
				((unsigned char *)&inaddr4->s_addr)[2],
				((unsigned char *)&inaddr4->s_addr)[3],
				result);
	}
}

void do_firedns_getip4(firedns_state *state, char * string) {
	int fd;
	fd_set s;
	int n;
	struct timeval tv;
	char *m;
	struct in_addr addr4;

	fprintf(stderr,"taking firedns_getip4(%s): ",string);
	fd = firedns_getip4(state, string);
	fprintf(stderr,"%d\n",fd);
	if (fd == -1)
		return;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&s);
	FD_SET(fd,&s);
	n = select(fd + 1,&s,NULL,NULL,&tv);
	(void) n;
	m = firedns_getresult(state, fd);
	if (m == NULL)
		fprintf(stderr,"getting result: (null)\n");
	else {
		memcpy(&addr4,m,sizeof(struct in_addr));
		char result[256];
		firedns_ntoa4(&addr4, result);
		fprintf(stderr,"getting result: %s\n", result);
	}
}

void do_firedns_getip6(firedns_state *state, char * string) {
	int fd;
	fd_set s;
	int n;
	struct timeval tv;
	char *m;
	struct in6_addr addr6;

	fprintf(stderr,"taking firedns_getip6(%s): ",string);
	fd = firedns_getip6(state, string);
	fprintf(stderr,"%d\n",fd);
	if (fd == -1)
		return;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&s);
	FD_SET(fd,&s);
	n = select(fd + 1,&s,NULL,NULL,&tv);
	(void) n;
	m = firedns_getresult(state, fd);
	if (m == NULL)
		fprintf(stderr,"getting result: (null)\n");
	else {
		memcpy(&addr6,m,sizeof(struct in6_addr));
		char result[256];
		firedns_ntoa6(&addr6, result);
		fprintf(stderr,"getting result: %s\n", result);
	}
}

void do_firedns_gettxt(firedns_state *state, char * string) {
	int fd;
	fd_set s;
	int n;
	struct timeval tv;
	char *m;

	fprintf(stderr,"taking firedns_gettxt(%s): ",string);
	fd = firedns_gettxt(state, string);
	fprintf(stderr,"%d\n",fd);
	if (fd == -1)
		return;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&s);
	FD_SET(fd,&s);
	n = select(fd + 1,&s,NULL,NULL,&tv);
	(void) n;
	m = firedns_getresult(state, fd);
	fprintf(stderr,"getting result: %s\n",m);
}

void do_firedns_getmx(firedns_state* state, char * string) {
	int fd;
	fd_set s;
	int n;
	struct timeval tv;
	char *m;

	fprintf(stderr,"taking firedns_getmx(%s): ",string);
	fd = firedns_getmx(state, string);
	fprintf(stderr,"%d\n",fd);
	if (fd == -1)
		return;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&s);
	FD_SET(fd,&s);
	n = select(fd + 1,&s,NULL,NULL,&tv);
	(void) n;
	m = firedns_getresult(state, fd);
	fprintf(stderr,"getting result: %s\n",m);
}

void do_firedns_getname4(firedns_state *state, char * string) {
	int fd;
	fd_set s;
	int n;
	struct timeval tv;
	char *m;
	struct in_addr buf4;
	struct in_addr *addr4;

	fprintf(stderr,"taking firedns_getname4(%s): ",string);
	addr4 = firedns_aton4(string, &buf4);
	if (addr4 == NULL) {
		fprintf(stderr,"(null)\n");
		return;
	}
	fd = firedns_getname4(state, addr4);
	fprintf(stderr,"%d\n",fd);
	if (fd == -1)
		return;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&s);
	FD_SET(fd,&s);
	n = select(fd + 1,&s,NULL,NULL,&tv);
	(void) n;
	m = firedns_getresult(state, fd);
	fprintf(stderr,"getting result: %s\n",m);
}

void do_firedns_getname6(firedns_state *state, char * string) {
	int fd;
	fd_set s;
	int n;
	struct timeval tv;
	char *m;
	struct in6_addr *addr6;
	struct in6_addr buf6;

	fprintf(stderr,"taking firedns_getname6(%s): ",string);
	addr6 = firedns_aton6(string, &buf6);
	if (addr6 == NULL) {
		fprintf(stderr,"(null)\n");
		return;
	}
	fd = firedns_getname6(state, addr6);
	fprintf(stderr,"%d\n",fd);
	if (fd == -1)
		return;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&s);
	FD_SET(fd,&s);
	n = select(fd + 1,&s,NULL,NULL,&tv);
	(void) n;
	m = firedns_getresult(state, fd);
	fprintf(stderr,"getting result: %s\n",m);
}

void do_firedns_aton6(firedns_state *state, char * string) {
	(void) state;
	struct in6_addr buf6;
	struct in6_addr *inaddr6;

	fprintf(stderr,"taking firedns_aton6(%s): ",string);
	inaddr6 = firedns_aton6(string, &buf6);
	if (inaddr6 == NULL)
		fprintf(stderr,"got NULL\n");
	else
		fprintf(stderr,"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
				inaddr6->s6_addr[0],
				inaddr6->s6_addr[1],
				inaddr6->s6_addr[2],
				inaddr6->s6_addr[3],
				inaddr6->s6_addr[4],
				inaddr6->s6_addr[5],
				inaddr6->s6_addr[6],
				inaddr6->s6_addr[7],
				inaddr6->s6_addr[8],
				inaddr6->s6_addr[9],
				inaddr6->s6_addr[10],
				inaddr6->s6_addr[11],
				inaddr6->s6_addr[12],
				inaddr6->s6_addr[13],
				inaddr6->s6_addr[14],
				inaddr6->s6_addr[15]);
}

#if 1
#warning "firedns_dnsbl_lookup_txt() not ported yet"
void do_firedns_dnsbl_lookup(firedns_state *state, char * ip, char * string) {
	(void) state, (void) ip, (void) string;
}
#else
void do_firedns_dnsbl_lookup(firedns_state *state, char * ip, char * string) {
	int fd;
	fd_set s;
	int n;
	struct timeval tv;
	char *m;
	struct in_addr *i;
	struct in_addr buf4;

	i = firedns_aton4(ip, &buf4);

	fprintf(stderr,"taking firedns_dnsbl_lookup(%s,%s): ",ip,string);
	fd = firedns_dnsbl_lookup_txt(state, i,string);
	fprintf(stderr,"%d\n",fd);
	if (fd == -1)
		return;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&s);
	FD_SET(fd,&s);
	n = select(fd + 1,&s,NULL,NULL,&tv);
	(void) n;
	m = firedns_getresult(state, fd);
	fprintf(stderr,"getting result: %s\n",m);
}
#endif

int main() {
	firedns_state state, *s = &state;
        firedns_init(s);
        firedns_add_servers_from_resolv_conf(s);

	do_firedns_aton4(s, "1.2.3.4");
	do_firedns_aton4(s, "101.102.103.104");
	do_firedns_aton4(s, "bah");
	do_firedns_aton4(s, "1.2.3.400");
	do_firedns_aton4(s, "206.152.182.152");
	do_firedns_aton4(s, "206.152.182.152)");
	do_firedns_aton4(s, "ack started...");

	do_firedns_aton6(s, "::");
	do_firedns_aton6(s, "3ffe:b80:346:1:204:5aff:fece:7852");
	do_firedns_aton6(s, "fe80::204:5aff:fece:7852");
	do_firedns_aton6(s, "bah");

	do_firedns_getip4(s, "celery.n.ml.org");
	do_firedns_getip4(s, "pp.info.uni-karlsruhe.de");
	do_firedns_getip4(s, "ftp.kernel.org");
	do_firedns_getip4(s, "pokr.com");

	do_firedns_getip6(s, "ftp.stealth.net");
	do_firedns_getip6(s, "www.ipv6.org");
	do_firedns_getip6(s, "z.ip6.int");

	do_firedns_gettxt(s, "2668.8086.pci.id.ucw.cz");
	do_firedns_gettxt(s, "2.0.0.127.outputs.orbz.org");
	do_firedns_gettxt(s, "2.0.0.127.bl.spamcop.net");
	do_firedns_gettxt(s, "2.0.0.127.relays.ordb.org");

	do_firedns_getmx(s, "spamcop.net");
	do_firedns_getmx(s, "penguinhosting.net");
	do_firedns_getmx(s, "taconic.net");
	do_firedns_getmx(s, "microsoft.com");
	do_firedns_getmx(s, "mail.ru");
	do_firedns_getmx(s, "cool.dk");

	do_firedns_getname4(s, "8.8.8.8");
	do_firedns_getname4(s, "208.171.237.190");
	do_firedns_getname4(s,"64.28.67.150");
	do_firedns_getname4(s, "64.90.162.91");

	do_firedns_getname6(s, "2a00:1450:400c:c05::1b");
	do_firedns_getname6(s, "2001:660:1180:1:192:134:0:49");
	do_firedns_getname6(s, "2001:6b0:e:2018::172");

	do_firedns_dnsbl_lookup(s, "127.0.0.2","list.dsbl.org");

	return 0;
}
