#include "firestring_internal.h"

void firestring_conf_free(struct firestring_conf_t * config) {
	struct firestring_conf_t *next;
	while (config != NULL) {
		next = config->next;
		free(config->var);
		free(config->value);
		free(config);
		config = next;
	}
}

