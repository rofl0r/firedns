#include "firestring_internal.h"

char *firestring_concat (const char * restrict const s, ...) {
	const char *curr;
	size_t len = 0;
	va_list va;
	char * restrict ret = NULL;
	
	va_start(va,s);
	curr = s;
	while (curr != NULL) {
		len += strlen(curr);
		curr = va_arg(va, const char *);
	}
	va_end(va);
	
	if (len) {
		ret = firestring_malloc(len + 1);
		*ret = '\0';
	}
	
	va_start(va,s);
	curr = s;
	while (curr != NULL) {
		strcat(ret,curr);
		curr = va_arg(va, const char *);
	}
	va_end(va);
	return ret;
}

