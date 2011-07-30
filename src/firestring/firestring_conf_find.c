#include "firestring_internal.h"

char *firestring_conf_find(const struct firestring_conf_t * restrict config, const char * restrict const var) { 
	return firestring_conf_find_next(config,var,NULL);
}

