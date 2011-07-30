#include "firestring_internal.h"

void *firestring_malloc(const size_t size) {
	char *output;
	output = malloc(size);
	if (output == NULL)
		error_handler();
	return output;
}

