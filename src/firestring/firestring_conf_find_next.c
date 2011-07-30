#include "firestring_internal.h"

char *firestring_conf_find_next(const struct firestring_conf_t * restrict config, const char * restrict const var, const char * const prev) {
	int canreturn = 0;
	if (prev == NULL)
		canreturn = 1;
	while (config != NULL) {
		if (firestring_strcasecmp(config->var, var) == 0) {
			if (canreturn == 1)
				return config->value;
			else if (config->value == prev)
				canreturn = 1;
		}
		config = config->next;
	}
	return NULL;
}

