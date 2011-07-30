#include "firestring_internal.h"

struct firestring_conf_t *firestring_conf_parse_next(const char * const filename, struct firestring_conf_t *prev) {
	struct firestring_conf_t *conf = prev;
	char buf[512], *ctx = NULL;
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp != NULL) {
		while (fgets(buf, sizeof(buf), fp) != NULL) {
			firestring_conf_parse_line(buf, default_keywords, &conf, &ctx);
		}
		fclose(fp);
	}
	return conf;
}

