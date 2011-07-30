#include "firestring_internal.h"

char *firestring_chomp(char *s) { 
	char *end = s;
	if (end != NULL) {
		end = &s[strlen(s) - 1];
		while (isspace(*end)) {
			*end = '\0';
			if (end == s)
				break;
			end--;
		}
	}
	return s;
}

