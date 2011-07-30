#include "firestring_internal.h"

int firestring_estr_read(struct firestring_estr_t * const f, const int fd) {
	long i;
	if (f->l == f->a)
		return 2;
	i = read(fd,&f->s[f->l],f->a - f->l);
	if (i == -1 && errno == EAGAIN)
		return 0;
	if (i <= 0)
		return 1;
	f->l += i;
	return 0;
}

