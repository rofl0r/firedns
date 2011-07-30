#include "firedns_internal.h"

struct in_addr *firedns_aton4_s(const char * restrict const ipstring, struct in_addr * restrict const ip) { 
	unsigned char *myip;
	int i,part = 0;
	myip = (unsigned char *)ip;
	memset(myip,'\0',4);
	for (i = 0; i < 16; i++) {
		switch (ipstring[i]) {
			case '\0':
				if (part != 3)
					return NULL;
				return ip;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (myip[part] > 25)
					return NULL;
				myip[part] *= 10;
				if (myip[part] == 250 && ipstring[i] - '0' > 6)
					return NULL;
				myip[part] += ipstring[i] - '0';
				break;
			case '.':
				if (part == 3)
					return ip;
				else
					part++;
				break;
			default:
				if (part == 3)
					return ip;
				else
					return NULL;
				break;
		}
	}
	if (part == 3)
		return ip;
	else
		return NULL;
}

