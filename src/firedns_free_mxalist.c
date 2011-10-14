#include "firedns_internal.h"

void firedns_free_mxalist(struct firedns_mxlist *list) {
	struct firedns_mxlist *iter;
	iter = list;
	while (iter != NULL) {
		if (iter->cname != NULL)
			free(iter->cname);
		if (iter->ip4list != NULL)
			free(iter->ip4list);
		if (iter->ip6list != NULL)
			free(iter->ip6list);
		iter = iter->next;
	}
	free(list);
}

