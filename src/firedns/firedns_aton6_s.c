#include "firedns_internal.h"

struct in6_addr *firedns_aton6_s(const char* ipstring, struct in6_addr* ip) {
	
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

