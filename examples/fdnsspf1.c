#include <stdio.h>
#include <string.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	struct firedns_txtlist *iter;
	char *spfrecord = NULL;

	if (argc != 2) {
		fprintf(stderr,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}

	iter = firedns_resolvetxtlist(argv[1]);

	if (iter == NULL)
		return 1;

	while (iter != NULL) {
		if (strncmp(iter->txt,"v=spf1 ",7) == 0) {
			if (spfrecord != NULL)
				return 1;
			spfrecord = iter->txt;
		}
		iter = iter->next;
	}

	if (spfrecord == NULL)
		return 1;

	printf("%s\n",spfrecord);

	return 0;
}
