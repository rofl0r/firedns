#include "firestring_internal.h"

int firestring_strncasecmp(const char * restrict const s1, const char * restrict const s2, const long n) {
	long s;
	for (s = 0; s < n; s++) {
		if (tolower((unsigned char) s1[s]) != tolower((unsigned char) s2[s])) {
			if (tolower((unsigned char) s1[s]) < tolower((unsigned char) s2[s]))
				return -1;
			else
				return 1;
		}
		if (s1[s] == '\0')
			return 0;
	}
	return 0;
}

