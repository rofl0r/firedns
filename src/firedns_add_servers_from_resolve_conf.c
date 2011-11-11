#include "firedns_internal.h"
#include "../../lib/include/fileparser.h"

int firedns_add_servers_from_resolve_conf(firedns_state* self) { 
	fileparser ff, *f = &ff;
	size_t i;
	int fails = 0;
	
	if(fileparser_open(f, FDNS_CONFIG_FBCK))
		return -1;
	
	while(!fileparser_readline(f)) {
		if (strncmp(f->buf, "nameserver", 10) == 0) {
			i = 10;
			while (i < f->len && (f->buf[i] == ' ' || f->buf[i] == '\t'))
				i++;
			
			fails += firedns_add_server(self, &f->buf[i]);
		}
	}

	fileparser_close(f);
	
	return fails;
}

