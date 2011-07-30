#include "firestring_internal.h"

void firestring_int_errorhandler() {
	perror("Allocation failed");
	exit(EXIT_FAILURE);
}

