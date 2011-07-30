#include "firestring_internal.h"

void firestring_set_error_handler(void (*e)()) {
	error_handler = e;
}

