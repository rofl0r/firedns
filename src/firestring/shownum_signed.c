#include "firestring_internal.h"

int shownum_signed(const long long n, const int padzero, const int numpad, char * const numbuf, const int space) {
	unsigned long long m;
	int i = 0;
	m = (unsigned long long) (n < 0 ? n * -1 : n);
	if ((long long) m != n) {
		numbuf[0] = '-';
		i = shownum_unsigned(m,padzero,numpad - 1,&numbuf[1],space - 1);
		if (i == -1)
			return -1;
		else
			return i + 1;
	} else
		return shownum_unsigned(m,padzero,numpad,numbuf,space);
}

