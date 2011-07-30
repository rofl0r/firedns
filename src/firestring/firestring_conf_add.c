#include "firestring_internal.h"

struct firestring_conf_t *firestring_conf_add(struct firestring_conf_t * restrict const next, const char * restrict const var, const char * restrict const value) { 
	struct firestring_conf_t *conf;
	if (var != NULL && value != NULL) {
		conf = firestring_malloc(sizeof(*conf));
		conf->next = next;
		conf->var = firestring_strdup(var);
		conf->value = firestring_strdup(value);
	} else
		conf = next;
	return conf;
}

