#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "../include/firedns.h"

#define max(a,b) (a > b ? a : b)

int checkjunk(struct in_addr *ip) {
	unsigned char *cip = (unsigned char *)ip;
	if (cip[0] == 10)
		return 1;
	if (cip[0] == 172 && (cip[1] >= 16 && cip[1] <= 31))
		return 1;
	if (cip[0] == 192 && cip[1] == 168)
		return 1;
	if (cip[0] == 192 && cip[1] == 0 && cip[2] == 2)
		return 1;
	if (cip[0] == 198 && (cip[1] == 18 || cip[1] == 19))
		return 1;
	if (cip[0] == 127)
		return 1;
	if (cip[0] >= 224 && cip[0] <= 240)
		return 1;
	if (cip[0] == 0)
		return 1;

	return 0;
}

int checkjunk6(struct in6_addr *ip) {
	if (memcmp(ip,"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01",16) == 0)
		return 1;
	if (memcmp(ip,"\xfe\x80",2) == 0)
		return 1;
	/* FIXME: there's gotta be more of these */
	return 0;
}

/* 0 - Everything looks good
 * 1 - An MX has a non-routable IP
 * 2 - An MX doesn't exist
 * 3 - An MX is a CNAME
 * 4 - An MX is an IP address
 *
 * 100 - Invalid syntax
 * 103 - DNS timeout
 */
int main(int argc, char **argv) {
#if 1
	printf("error. mxalist not ported yet");
	return 1;
#else
	struct firedns_mxlist *iter, *list;
	struct in6_addr buf6;
	struct in_addr buf4;
	char ntoabuf[256];
	int ret = 0;
	int xml = 0;

	if (argc == 3 && argv[1][0] == '-') {
		int i = 1;
		while (argv[1][i] != '\0') {
			switch (argv[1][i]) {
				case 'x':
					xml = 1;
					break;
			}
			i++;
		}
		argv[1] = argv[2];
		argc--;
	}

	if (argc != 2) {
		fprintf(stderr,"usage: [-x] %s <hostname>\n",argv[0]);
		return 100;
	}

	list = iter = firedns_resolvemxalist(argv[1]);

	if (iter == NULL)
		return 103;

	if (xml)
		printf("<?xml version=\"1.0\"?>\n"
"<fdnsmxalist time=\"%d\" hostname=\"%s\">\n",(int)time(NULL),argv[1]);

	while (iter != NULL) {
		if (xml)
			printf("	<mx hostname=\"%s\" protocol=\"%s\" priority=\"%d\" port=\"%d\">\n",iter->name,firedns_mx_name[iter->protocol],iter->priority,firedns_mx_port[iter->protocol]);
		else
			printf("%7s (%05d) %s:%d\n",firedns_mx_name[iter->protocol],iter->priority,iter->name,firedns_mx_port[iter->protocol]);
		if (iter->cname != NULL) {
			if (xml)
				printf("		<error type=\"cname\" cname=\"%s\" />\n",iter->cname);
			else
				printf("ERROR: %s is a CNAME for %s\n",iter->name,iter->cname);
			ret = max(ret,3);
		}
		if (firedns_aton4(iter->name, &buf4) != NULL) {
			if (xml)
				printf("		<error type=\"ip4_mx\" />\n");
			else
				printf("ERROR: %s is an IPv4 address\n",iter->name);
			ret = max(ret,4);
			goto wrap;
		}
		if (firedns_aton6(iter->name, &buf6) != NULL) {
			if (xml)
				printf("		<error type=\"ip6_mx\" />\n");
			else
				printf("ERROR: %s is an IPv6 address\n",iter->name);
			ret = max(ret,4);
			goto wrap;
		}
		if (iter->ip4list == NULL && iter->ip6list == NULL) {
			if (xml)
				printf("		<error type=\"nxdomain\" />\n");
			else
				printf("ERROR: %s does not exist\n",iter->name);
			ret = max(ret,2);
		} else {
			struct firedns_ip4list *ipiter = iter->ip4list;
			struct firedns_ip6list *ip6iter = iter->ip6list;

			while (ipiter) {
				if (xml)
					printf("		<ip version=\"4\">%s",firedns_ntoa4(&ipiter->ip, ntoabuf));
				else
					printf("             %s\n",firedns_ntoa4(&ipiter->ip, ntoabuf));
				if (checkjunk(&ipiter->ip)) {
					if (xml)
						printf("\n			<error type=\"private-ip\" />\n		");
					else
						printf("ERROR: %s has an IP of %s, which is non-routable\n",iter->name,firedns_ntoa4(&ipiter->ip, ntoabuf));
					ret = max(ret,1);
				}
				if (xml)
					printf("</ip>\n");
				ipiter = ipiter->next;
			}

			while (ip6iter) {
				if (xml)
					printf("		<ip version=\"6\">%s",firedns_ntoa6(&ip6iter->ip, ntoabuf));
				else
					printf("             %s\n",firedns_ntoa6(&ip6iter->ip, ntoabuf));
				if (checkjunk6(&ip6iter->ip)) {
					if (xml)
						printf("\n			<error type=\"private-ip\" />\n		");
					else
						printf("ERROR: %s has an IP of %s, which is non-routable\n",iter->name,firedns_ntoa6(&ip6iter->ip, ntoabuf));
					ret = max(ret,1);
				}
				if (xml)
					printf("</ip>\n");
				ip6iter = ip6iter->next;
			}
		}
wrap:
		if (xml)
			printf("	</mx>\n");
		iter = iter->next;
	}

	/* this is more to make sure freeing works than anything else */
	firedns_free_mxalist(list);

	if (xml)
		printf("</fdnsmxalist>\n");

	return ret;
#endif
}
