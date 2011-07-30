#include "firestring_internal.h"

enum firestring_conf_parse_line_result firestring_conf_parse_line(const char *line, const struct firestring_conf_keyword_t keywords[], struct firestring_conf_t **conf, char **context) {
	enum firestring_conf_parse_line_result err;
	char *curr, *ptr, *var = NULL, *val = NULL;
	int i;
	if (context == NULL || conf == NULL)
		return FSC_MALFORMED;
	
	curr = firestring_concat(*context ? *context : "", line, NULL);
	ptr = firestring_chug(curr);
	
	for (i=0; keywords[i].keyword != NULL; i++) {
		if (strncmp(keywords[i].keyword, ptr, strlen(keywords[i].keyword)) == 0) {
			if (keywords[i].callback != NULL)
				err = keywords[i].callback(ptr + strlen(keywords[i].keyword), conf);
			else
				err = FSC_VALID;
			goto done;
		}
	}
	
	val = strchr(ptr, '=');
	if (val == NULL) {
		err = FSC_MALFORMED;
		goto done;
	}
	*val++ = '\0';
	var = firestring_chomp(ptr);
	
	err = strip_quotes(&val);
done:
	if (err == FSC_INCOMPLETE) {
		ptr = *context;
		*context = firestring_concat(*context ? *context : "", line, NULL);
		if (ptr)
			free(ptr);
	}
	else if (*context != NULL) {
		free(*context);
		*context = NULL;
	}
	if (err == FSC_VALID) {
		*conf = firestring_conf_add(*conf, var, val);
	}
	free(curr);
	return err;
}

