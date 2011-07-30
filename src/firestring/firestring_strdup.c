#include "firestring_internal.h"

char *firestring_strdup(const char * restrict const input) { 
	char * restrict output;
	size_t s;
	if (input == NULL)
		return NULL;
	s = strlen(input) + 1;
	output = firestring_malloc(s);
	memcpy(output,input,s);
	return(output);
}

