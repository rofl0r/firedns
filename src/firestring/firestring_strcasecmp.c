#include "firestring_internal.h"

int firestring_strcasecmp(const char * restrict const s1, const char * restrict const s2) { 
	size_t s;
	char c1, c2;
	for (s = 0; (s1[s] != '\0') && (s2[s] != '\0') && (tolower((unsigned char) s1[s]) == tolower((unsigned char) s2[s])); s++);
	c1 = tolower((unsigned char) s1[s]);
	c2 = tolower((unsigned char) s2[s]);
	if (c1 == c2)
		return 0;
	else if (c1 < c2)
		return -1;
	else
		return 1;
}

