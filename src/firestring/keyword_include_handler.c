#include "firestring_internal.h"

enum firestring_conf_parse_line_result keyword_include_handler(char *line, struct firestring_conf_t **conf) {
	char *sig;
	line = firestring_chug(firestring_chomp(line));
	sig = firestring_concat("inside_include_handler_", line, NULL);
	if (firestring_conf_find(*conf, sig) == NULL) {
		*conf = firestring_conf_add(*conf, sig, "");
		*conf = firestring_conf_parse_next(line, *conf);
		*conf = firestring_conf_delete(*conf, sig);
	}
	free(sig);
	return FSC_VALID;
}

