#include "firestring_internal.h"

int firestring_estr_xml_decode(struct firestring_estr_t * const t, const struct firestring_estr_t * const f) {
	long i = 0, o = 0;
	long j,k,l;
	int tempint;
	if (t->a < f->l)
		return 1;
	while ((j = firestring_estr_strchr(f,'&',i)) != -1) {
		memmove(&t->s[o],&f->s[i],j - i);
		o += j - i;
		if (f->s[j + 1] == '#') {
			
			k = firestring_estr_strchr(f,';',j + 1);
			if (k == -1) 
				return 1;
			if (f->s[j + 2] == 'x') {
				
				j += 3;
				for (l = j; l < k; l += 2) {
					tempint = firestring_hextoi(&f->s[l]);
					if (tempint == -1) 
						return 1;
					t->s[o++] = tempint;
				}
			} else {
				
				if (k - j < 3 || k - j > 5)
					return 1; 
				t->s[o++] = (char) atoi(&f->s[j + 2]);
			}
			i = k + 1;
		} else {
			
			for (l = 0; xml_decode_table[l].entity != NULL; l++) {
				k = strlen(xml_decode_table[l].entity);
				if (j + l < f->l && memcmp(&f->s[j],xml_decode_table[l].entity,k) == 0)
					break;
			}
			if (xml_decode_table[l].entity == NULL) 
				return 1;
			t->s[o++] = xml_decode_table[l].character;
			i = j + strlen(xml_decode_table[l].entity);
		}
	}
	memmove(&t->s[o],&f->s[i],f->l - i);
	t->l = o + (f->l - i);
	return 0;
}

