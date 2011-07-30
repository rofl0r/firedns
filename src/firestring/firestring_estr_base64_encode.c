#include "firestring_internal.h"

int firestring_estr_base64_encode(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f) {
	long i;
	unsigned char *s, *d;
	s = (unsigned char *)f->s;
	d = (unsigned char *)t->s;
	if (f->l * 4 / 3 > t->a - 4)
		return 1;
	t->l = 0;
	for (i = 0; i < f->l - 2; i += 3) {
		d[t->l++] = base64_encode_table[s[i] >> 2];
		d[t->l++] = base64_encode_table[(s[i] << 4 | s[i+1] >> 4) % 64];
		d[t->l++] = base64_encode_table[(s[i+1] << 2 | s[i+2] >> 6) % 64];
		d[t->l++] = base64_encode_table[s[i+2] % 64];
	}
	if (i == f->l - 1) {
		d[t->l++] = base64_encode_table[s[i] >> 2];
		d[t->l++] = base64_encode_table[(s[i] << 4) % 64];
		d[t->l++] = '=';
		d[t->l++] = '=';
	} else if (i == f->l - 2)  {
		d[t->l++] = base64_encode_table[s[i] >> 2];
		d[t->l++] = base64_encode_table[(s[i] << 4 | s[i+1] >> 4) % 64];
		d[t->l++] = base64_encode_table[(s[i+1] << 2) % 64];
		d[t->l++] = '=';
	}
	return 0;
}

