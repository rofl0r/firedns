#include "firestring_internal.h"

void *firestring_realloc(void * old, const size_t new) {
	char *output;
	output = realloc(old,new);
	if (output == NULL)
		error_handler();
	return output;
}

