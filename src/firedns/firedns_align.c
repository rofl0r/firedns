#include "firedns_internal.h"

void *firedns_align(void *inp) {
	char *p = inp;
	int offby = ((char *)p - (char *)0) % FIREDNS_ALIGN;
	if (offby != 0)
		return p + (FIREDNS_ALIGN - offby);
	else
		return p;
}

