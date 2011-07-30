#include "firestring_internal.h"

int firestring_hextoi(const char * const input) {
	char o1, o2;
	o1 = hex_decode_table[(int)input[0]];
	if (o1 == 16)
		return -1;
	o2 = hex_decode_table[(int)input[1]];
	if (o2 == 16)
		return -1;
	return (o1 << 4) | o2;
}

