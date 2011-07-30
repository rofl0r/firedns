#include "firestring_internal.h"

int shownum_unsigned(unsigned long long m, const int padzero, int numpad, char * const numbuf, const int space) {
	int power; 
	long long multiple = 1;
	int r;
	char f;
	int i = 0;
	for (power = 0; multiple <= m; power++)
		multiple *= 10;
	
	power = max(power,1);
	multiple = max(multiple,10);
	f = padzero == 1 ? '0' : ' ';
	if (power > space || numpad > space)
		return -1;
	if (power < numpad) {
		numpad -= power;
		for (; i < numpad; i++)
			numbuf[i] = f;
	}
	for (;power > 0; power--) {
		multiple /= 10;
		r = m / multiple;
		numbuf[i++] = '0' + r;
		m -= r * multiple;
	}
	return i;
}

