#include "firestring_internal.h"

int shownum_fsigned(const double n, const int padzero, const int numpad, char * const numbuf, const int space) {
	double m;
	int i = 0;
	m = n < 0.0 ? n * -1.0 : n;
	if (m != n) {
		numbuf[0] = '-';
		i = shownum_funsigned(m,padzero,numpad - 1,&numbuf[1],space - 1);
		if (i == -1)
			return -1;
		else
			return i + 1;
	} else
		return shownum_funsigned(m,padzero,numpad,numbuf,space);
}

