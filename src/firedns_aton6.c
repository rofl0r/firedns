#include "firedns_internal.h"
#include <string.h>

const char hex_decode_table[] = {
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 16, 16, 16, 16, 16, 16,
	16, 10, 11, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 10, 11, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};

static int firestring_hextoi(const char* input) {
	char o1, o2;
	o1 = hex_decode_table[(int)input[0]];
	if (o1 == 16)
		return -1;
	o2 = hex_decode_table[(int)input[1]];
	if (o2 == 16)
		return -1;
	return (o1 << 4) | o2;
}

struct in6_addr *firedns_aton6(const char* ipstring, struct in6_addr* ip) {
	
	char instring[40];
	char tempstr[5];
	int i,o;
	int direction = 1;
	char *tempchr,*tempchr2;;
	i = strlen(ipstring);
	if (i > 39)
		return NULL;
	memcpy(instring,ipstring,i+1);
	memset(ip->s6_addr,'\0',16);
	tempchr2 = instring;
	i = 0;
	while (direction > 0) {
		if (direction == 1) {
			tempchr = strchr(tempchr2,':');
			if (tempchr == NULL && i != 14)
				return NULL;
			if (tempchr != NULL)
				tempchr[0] = '\0';
			o = strlen(tempchr2);
			if (o > 4)
				return NULL;
			strcpy(tempstr,"0000");
			strcpy(&tempstr[4 - o],tempchr2);
			o = firestring_hextoi(tempstr);
			if (o == -1)
				return NULL;
			ip->s6_addr[i++] = o;
			o = firestring_hextoi(&tempstr[2]);
			if (o == -1)
				return NULL;
			ip->s6_addr[i++] = o;
			if (i >= 15)
				break;
			tempchr2 = tempchr + 1;
			if (tempchr2[0] == ':') {
				tempchr2++;
				direction = 2;
				i = 15;
				continue;
			}
		}
		if (direction == 2) {
			tempchr = strrchr(tempchr2,':');
			if (tempchr == NULL)
				tempchr = tempchr2;
			else {
				tempchr[0] = '\0';
				tempchr++;
			}
			o = strlen(tempchr);
			if (o > 4)
				return NULL;
			strcpy(tempstr,"0000");
			strcpy(&tempstr[4 - o],tempchr);
			o = firestring_hextoi(&tempstr[2]);
			if (o == -1)
				return NULL;
			ip->s6_addr[i--] = o;
			o = firestring_hextoi(tempstr);
			if (o == -1)
				return NULL;
			ip->s6_addr[i--] = o;
			if (i <= 1)
				break;
			if (tempchr == tempchr2)
				break;
		}
	}
	return ip;
}

