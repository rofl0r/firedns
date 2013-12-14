#include "firedns_internal.h"
#include <string.h>
#ifdef USE_LIBULZ
#include <ulz/fileparser.h>
#else
#include <stdio.h>
#include <ctype.h>
#endif

int firedns_add_servers_from_resolv_conf(firedns_state* self) {
	size_t i;
	int fails = 0;

#ifdef USE_LIBULZ
	fileparser ff, *f = &ff;

	if(fileparser_open(f, FDNS_CONFIG_FBCK)) return -1;

	#define LINE f->buf
	#define LINESIZE f->len

	while(!fileparser_readline(f)) {
		if (strncmp(LINE, "nameserver", 10) == 0) {
			i = 10;
			while (i < LINESIZE && (LINE[i] == ' ' || LINE[i] == '\t')) i++;
			fails += firedns_add_server(self, LINE+i);
		}
	}

	fileparser_close(f);
#else
	FILE *f;
	char *l, buf[256];
	if(!(f = fopen(FDNS_CONFIG_FBCK, "r"))) return -1;
	while((l = fgets(buf, sizeof(buf), f))) {
		if (strncmp(l, "nameserver", 10) == 0) {
			i = 10;
			while(isspace(l[i])) i++;
			size_t j = i;
			while(l[j] && l[j] != '\n') j++;
			l[j] = 0;
			fails += firedns_add_server(self, l+i);
		}
	}

	fclose(f);
#endif
	return fails;
}

