#include "firestring_internal.h"

struct firestring_conf_t *firestring_conf_parse(const char * const filename) {
	return firestring_conf_parse_next(filename, NULL);
}

