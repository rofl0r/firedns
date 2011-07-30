#include "firestring_internal.h"

void firestring_strncat(char * restrict const to, const char * restrict const from, const size_t size) {
	size_t l;
	l = strlen(to);
	firestring_strncpy(&to[l],from,size - l);
	return;
}

