#include "firestring_internal.h"

char *firestring_chug(char * s) { 
	if (s != NULL) {
		while (isspace(*s))
			s++;
	}
	return s;
}

