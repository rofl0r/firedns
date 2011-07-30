#include "firestring_internal.h"

long firestring_vfprintfe(FILE *stream, const struct firestring_estr_t * restrict const format, va_list ap) {
	size_t f,tl;
	const char *tempchr;
	const struct firestring_estr_t *e;
	int padzero, numpad;
#define NUMSTRING_MAX 256
	char numbuf[NUMSTRING_MAX];
	int out = 0;
	int m;
	time_t t;
#undef SIGNED_NUMBER
#undef UNSIGNED_NUMBER
#undef FSIGNED_NUMBER
#define SIGNED_NUMBER(type) \
	m = shownum_signed((long long) va_arg(ap,type),padzero,numpad,numbuf,NUMSTRING_MAX); \
	out += m; \
	fwrite(numbuf,m,1,stream);
#define UNSIGNED_NUMBER(type) \
	m = shownum_unsigned((unsigned long long) va_arg(ap,type),padzero,numpad,numbuf,NUMSTRING_MAX); \
	out += m; \
	fwrite(numbuf,m,1,stream);
#define FSIGNED_NUMBER(type) \
	m = shownum_fsigned((double) va_arg(ap,type),padzero,numpad,numbuf,NUMSTRING_MAX); \
	out += m; \
	fwrite(numbuf,m,1,stream);
	for (f = 0; f < format->l; f++) {
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
					out += tl;
					fwrite(tempchr,tl,1,stream);
					break;
				case 'e':
					e = va_arg(ap,const struct firestring_estr_t *);
					if (e == NULL)
						break;
					out += e->l;
					fwrite(e->s,e->l,1,stream);
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
					m = showdate(numbuf,NUMSTRING_MAX,t);
					fwrite(numbuf,m,1,stream);
					out += m;
					break;
				case '%':
					out++;
					fwrite("%",1,1,stream);
					break;
			}
		} else {
			out++;
			fwrite(&format->s[f],1,1,stream);
		}
	}
	return out;
}

