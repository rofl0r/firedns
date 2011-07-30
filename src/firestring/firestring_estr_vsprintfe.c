#include "firestring_internal.h"

long firestring_estr_vsprintfe(struct firestring_estr_t * restrict const o, const struct firestring_estr_t * restrict const format, va_list ap) {
	size_t f,tl;
	const char *tempchr;
	const struct firestring_estr_t *e;
	int b = 0;
	int padzero, numpad;
	int m;
	time_t t;
#define SIGNED_NUMBER(type) \
	m = shownum_signed((long long) va_arg(ap,type),padzero,numpad,&o->s[o->l],o->a - o->l); \
	if (m == -1) \
		b = 1; \
	else \
		o->l += m;
#define UNSIGNED_NUMBER(type) \
	m = shownum_unsigned((unsigned long long) va_arg(ap,type),padzero,numpad,&o->s[o->l],o->a - o->l); \
	if (m == -1) \
		b = 1; \
	else \
		o->l += m;
#define FSIGNED_NUMBER(type) \
	m = shownum_fsigned((double) va_arg(ap,type),padzero,numpad,&o->s[o->l],o->a - o->l); \
	if (m == -1) \
		b = 1; \
	else \
		o->l += m;
	o->l = 0;
	for (f = 0; f < format->l && o->l < o->a && b == 0; f++) {
		if (format->s[f] == '%') {
			padzero = 0;
			numpad = 0;
eformatcheck:
			switch(format->s[++f]) {
				case '0':
					if (numpad == 0 && padzero == 0) {
						padzero = 1;
						goto eformatcheck;
					}
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					numpad = (numpad * 10) + (format->s[f] - '0');
					goto eformatcheck;
				case 's':
					tempchr = va_arg(ap,const char *);
					if (tempchr == NULL)
						tempchr = "(null)";
					tl = strlen(tempchr);
					if (tl + o->l > o->a)
						b = 1;
					else {
						memcpy(&o->s[o->l],tempchr,tl);
						o->l += tl;
					}
					break;
				case 'e':
					e = va_arg(ap,const struct firestring_estr_t *);
					if (e == NULL)
						break;
					if (e->l + o->l > o->a)
						b = 1;
					else {
						memcpy(&o->s[o->l],e->s,e->l);
						o->l += e->l;
					}
					break;
				case 'd': 
					SIGNED_NUMBER(int)
					break;
				case 'l': 
					SIGNED_NUMBER(long)
					break;
				case 'u': 
					UNSIGNED_NUMBER(unsigned int)
					break;
				case 'y': 
					UNSIGNED_NUMBER(unsigned long)
					break;
				case 'g': 
					SIGNED_NUMBER(long long)
					break;
				case 'o': 
					UNSIGNED_NUMBER(unsigned long long)
					break;
				case 'f': 
					FSIGNED_NUMBER(double);
					break;
				case 't': 
					t = va_arg(ap,time_t);
					m = showdate(&o->s[o->l],o->a - o->l,t);
					if (m == -1)
						b = 1;
					else
						o->l += m;
					break;
				case '%':
					o->s[o->l++] = '%';
					break;
			}
		} else
			o->s[o->l++] = format->s[f];
	}
	return o->l;
}

