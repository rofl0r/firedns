#include "firestring_internal.h"

int firestring_estr_base64_decode(struct firestring_estr_t * const t, const struct firestring_estr_t * const f) {
	long i,j,o;
	unsigned char tempblock[4];
	if (f->l * 3 / 4 > t->a - 3)
		return 1;
	o = 0;
	for (i = 0; i < f->l - 3; i += 4) {
		for (j = 0; j < 4; j++) {
			tempblock[j] = (unsigned char) f->s[i + j];
			if (base64_decode_table[(int)tempblock[j]] == 64) {
				if (++i > f->l - 4)
					goto base64_end;
				j--;
				continue;
			}
		}
		t->s[o++] = base64_decode_table[(int)tempblock[0]] << 2 | base64_decode_table[(int)tempblock[1]] >> 4;
		t->s[o++] = base64_decode_table[(int)tempblock[1]] << 4 | base64_decode_table[(int)tempblock[2]] >> 2;
		t->s[o++] = base64_decode_table[(int)tempblock[2]] << 6 | base64_decode_table[(int)tempblock[3]];
	}
	if (tempblock[3] == '=')
		o--;
	if (tempblock[2] == '=')
		o--;
base64_end:
	t->l = o;
	return 0;
}

