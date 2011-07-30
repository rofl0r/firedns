#include "firestring_internal.h"

int firestring_estr_xml_encode(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f) {
	long i,j,k;
	if (t->a < f->l * XML_WORSTCASE)
		return 1;
	t->l = 0;
	for (i = 0; i < f->l; i++) {
		if (strchr(XML_UNSAFE,f->s[i])) {
			for (j = 0; xml_decode_table[j].entity != NULL; j++)
				if (xml_decode_table[j].character == f->s[i])
					break;
			if (xml_decode_table[j].entity == NULL)
				return 1;
			k = strlen(xml_decode_table[j].entity);
			memcpy(&t->s[t->l],xml_decode_table[j].entity,k);
			t->l += k;
		} else
			t->s[t->l++] = f->s[i];
	}
	return 0;
}

