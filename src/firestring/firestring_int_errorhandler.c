#include "firestring_internal.h"
#include "../../../lib/include/logger.h"

void firestring_int_errorhandler() {
	log_perror("Allocation failed");
	exit(EXIT_FAILURE);
}

