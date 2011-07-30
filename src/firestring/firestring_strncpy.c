#include "firestring_internal.h"

void firestring_strncpy(char * restrict const to, const char * restrict const from, const size_t size) {
	strncpy(to,from,size);
	to[size - 1]= '\0';
	return;
}

