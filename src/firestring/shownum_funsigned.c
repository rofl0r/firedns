#include "firestring_internal.h"

int shownum_funsigned(const double m, const int padzero, int numpad, char * const numbuf, const int space) {
	char f;
	int i,j;
	f = padzero == 1 ? '0' : ' ';
	if (numpad > space)
		return -1;
	j = numpad;
	if (j == 0)
		j = 6;
	if (space < j)
		j = space;
#ifdef HAVE_GCVT
	gcvt((const double) m,j,numbuf);
#else
	numbuf[0] = '\0';
#endif
	i = strlen(numbuf);
	if (i >= numpad)
		return i;
	memmove(&numbuf[numpad - i],numbuf,i);
	for (i = numpad - i; i; i--)
		numbuf[i-1] = f;
	return numpad;
}

