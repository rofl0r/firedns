#include "firestring_internal.h"

struct firestring_conf_t *firestring_conf_delete(struct firestring_conf_t *conf, const char *var) {
	struct firestring_conf_t *prev = NULL, *head = conf;
	while (conf != NULL) {
		if (firestring_strcasecmp(conf->var, var) == 0) {
			free(conf->var);
			free(conf->value);
			if (prev)
				prev->next = conf->next;
			if (head == conf)
				head = conf->next;
			free(conf);
			break;
		}
		prev = conf;
		conf = conf->next;
	}
	return head;
}

