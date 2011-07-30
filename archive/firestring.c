/*
firestring.c - FireString replacement string functions
Copyright (C) 2000 Ian Gulliver

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

Parts of this library are taken from dstr and dconf, part of LibD:
* Copyright (C) 2001-2002  Andres Salomon <dilinger@voxel.net>

*/

#define _FIRESTRING_C

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include "firemake.h"
#include "firestring.h"

#define max(a,b) (a > b ? a : b)
#define min(a,b) (a < b ? a : b)

static void firestring_int_errorhandler();

static const char tagstring[] = "$Id: firestring.c,v 1.107 2004/03/09 14:47:53 ian Exp $";
const char firestring_version[] = VERSION;

static void (*error_handler)() = firestring_int_errorhandler;

struct xml_encoding {
	char character;
	char *entity;
};

static const char base64_encode_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
	};

static const char base64_decode_table[] = {
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64,  0, 64, 64,
	64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
	};

static const char hex_decode_table[] = {
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 16, 16, 16, 16, 16, 16,
	16, 10, 11, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 10, 11, 12, 13, 14, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
	};

#define XML_UNSAFE "<>\"&"
#define XML_WORSTCASE 6

static const struct xml_encoding xml_decode_table[] = {
	{ '<', "&lt;" },
	{ '>', "&gt;" },
	{ '"', "&quot;" },
	{ '&', "&amp;" },
	{ ' ', "&nbsp;" },
	{ '\xa1', "&iexcl;" },
	{ '\xa2', "&cent;" },
	{ '\xa3', "&pound;" },
	{ '\xa4', "&curren;" },
	{ '\xa5', "&yen;" },
	{ '\xa6', "&brvbar;" },
	{ '\xa7', "&sect;" },
	{ '\xa8', "&uml;" },
	{ '\xa9', "&copy;" },
	{ '\xaa', "&ordf;" },
	{ '\xab', "&laquo;" },
	{ '\xac', "&not;" },
	{ '\xad', "&shy;" },
	{ '\xae', "&reg;" },
	{ '\xaf', "&macr;" },
	{ '\xb0', "&deg;" },
	{ '\xb1', "&plusmn;" },
	{ '\xb2', "&sup2;" },
	{ '\xb3', "&sup3;" },
	{ '\xb4', "&acute;" },
	{ '\xb5', "&micro;" },
	{ '\xb6', "&para;" },
	{ '\xb7', "&middot;" },
	{ '\xb8', "&cedil;" },
	{ '\xb9', "&sup1;" },
	{ '\xba', "&ordm;" },
	{ '\xbb', "&raquo;" },
	{ '\xbc', "&frac14;" },
	{ '\xbd', "&frac12;" },
	{ '\xbe', "&frac34;" },
	{ '\xbf', "&iquest;" },
	{ '\xc0', "&Agrave;" },
	{ '\xc1', "&Aacute;" },
	{ '\xc2', "&Acirc;" },
	{ '\xc3', "&Atilde;" },
	{ '\xc4', "&Auml;" },
	{ '\xc5', "&Aring;" },
	{ '\xc6', "&AElig;" },
	{ '\xc7', "&Ccedil;" },
	{ '\xc8', "&Egrave;" },
	{ '\xc9', "&Eacute;" },
	{ '\xca', "&Ecirc;" },
	{ '\xcb', "&Euml;" },
	{ '\xcc', "&Igrave;" },
	{ '\xcd', "&Iacute;" },
	{ '\xce', "&Icirc;" },
	{ '\xcf', "&Iuml;" },
	{ '\xd0', "&ETH;" },
	{ '\xd1', "&Ntilde;" },
	{ '\xd2', "&Ograve;" },
	{ '\xd3', "&Oacute;" },
	{ '\xd4', "&Ocirc;" },
	{ '\xd5', "&Otilde;" },
	{ '\xd6', "&Ouml;" },
	{ '\xd7', "&times;" },
	{ '\xd8', "&Oslash;" },
	{ '\xd9', "&Ugrave;" },
	{ '\xda', "&Uacute;" },
	{ '\xdb', "&Ucirc;" },
	{ '\xdc', "&Uuml;" },
	{ '\xdd', "&Yacute;" },
	{ '\xde', "&THORN;" },
	{ '\xdf', "&szlig;" },
	{ '\xe0', "&agrave;" },
	{ '\xe1', "&aacute;" },
	{ '\xe2', "&acirc;" },
	{ '\xe3', "&atilde;" },
	{ '\xe4', "&auml;" },
	{ '\xe5', "&aring;" },
	{ '\xe6', "&aelig;" },
	{ '\xe7', "&ccedil;" },
	{ '\xe8', "&egrave;" },
	{ '\xe9', "&eacute;" },
	{ '\xea', "&ecirc;" },
	{ '\xeb', "&euml;" },
	{ '\xec', "&igrave;" },
	{ '\xed', "&iacute;" },
	{ '\xee', "&icirc;" },
	{ '\xef', "&iuml;" },
	{ '\xf0', "&eth;" },
	{ '\xf1', "&ntilde;" },
	{ '\xf2', "&ograve;" },
	{ '\xf3', "&oacute;" },
	{ '\xf4', "&ocirc;" },
	{ '\xf5', "&otilde;" },
	{ '\xf6', "&ouml;" },
	{ '\xf7', "&divide;" },
	{ '\xf8', "&oslash;" },
	{ '\xf9', "&ugrave;" },
	{ '\xfa', "&uacute;" },
	{ '\xfb', "&ucirc;" },
	{ '\xfc', "&uuml;" },
	{ '\xfd', "&yacute;" },
	{ '\xfe', "&thorn;" },
	{ '\xff', "&yuml;" },
	{ '\0', NULL }
};

static void firestring_int_errorhandler() {
	perror("Allocation failed");
	exit(EXIT_FAILURE);
}

void firestring_set_error_handler(void (*e)()) {
	error_handler = e;
}

void *firestring_malloc(const size_t size) {
	char *output;
	output = malloc(size);
	if (output == NULL)
		error_handler();
	return output;
}

void *firestring_realloc(void * old, const size_t new) {
	char *output;
	output = realloc(old,new);
	if (output == NULL)
		error_handler();
	return output;
}

char *firestring_strdup(const char * restrict const input) { /* allocate and return pointer to duplicate string */
	char * restrict output;
	size_t s;
	if (input == NULL)
		return NULL;
	s = strlen(input) + 1;
	output = firestring_malloc(s);
	memcpy(output,input,s);
	return(output);
}

void firestring_strncpy(char * restrict const to, const char * restrict const from, const size_t size) {
	strncpy(to,from,size);
	to[size - 1]= '\0';
	return;
}

void firestring_strncat(char * restrict const to, const char * restrict const from, const size_t size) {
	size_t l;
	l = strlen(to);
	firestring_strncpy(&to[l],from,size - l);
	return;
}

static inline int showdate(char * const dest, const int space, const time_t t) {
	struct tm bt;
	int i;

	if (space < 18)
		return -1;

	localtime_r(&t,&bt);

	i = strftime(dest,18,"%Y%m%dT%H:%M:%S",&bt);
	
	if (i != 17)
		return -1;

	return 17;
}

static inline int shownum_unsigned(unsigned long long m, const int padzero, int numpad, char * const numbuf, const int space) {
	int power; /* smallest power of 10 greater than m */
	long long multiple = 1;
	int r;
	char f;
	int i = 0;

	for (power = 0; multiple <= m; power++)
		multiple *= 10;

	/* fixup 0 case, which shouldn't really display in technical terms */
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

static inline int shownum_funsigned(const double m, const int padzero, int numpad, char * const numbuf, const int space) {
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

static inline int shownum_signed(const long long n, const int padzero, const int numpad, char * const numbuf, const int space) {
	unsigned long long m;
	int i = 0;

	m = (unsigned long long) (n < 0 ? n * -1 : n);
	if ((long long) m != n) {
		numbuf[0] = '-';
		i = shownum_unsigned(m,padzero,numpad - 1,&numbuf[1],space - 1);
		if (i == -1)
			return -1;
		else
			return i + 1;
	} else
		return shownum_unsigned(m,padzero,numpad,numbuf,space);
}

static inline int shownum_fsigned(const double n, const int padzero, const int numpad, char * const numbuf, const int space) {
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

long firestring_fprintf(FILE * restrict stream, const char * restrict const format, ...) {
	va_list ap;
	long o;

	va_start(ap,format);
	o = firestring_vfprintfe(stream,&ESTR_D((char *)format),ap);
	va_end(ap);
	return o;
}

long firestring_fprintfe(FILE * restrict stream, const struct firestring_estr_t * restrict const format, ...) {
	va_list ap;
	long o;

	va_start(ap,format);
	o = firestring_vfprintfe(stream,format,ap);
	va_end(ap);
	return o;
}

long firestring_printf(const char * restrict const format, ...) {
	va_list ap;
	long o;

	va_start(ap,format);
	o = firestring_vfprintfe(stdout,&ESTR_D((char *)format),ap);
	va_end(ap);
	return o;
}

long firestring_printfe(const struct firestring_estr_t * restrict const format, ...) {
	va_list ap;
	long o;

	va_start(ap,format);
	o = firestring_vfprintfe(stdout,format,ap);
	va_end(ap);
	return o;
}

long firestring_snprintf(char * restrict const out, const long size, const char * restrict const format, ...) {
	va_list ap;
	struct firestring_estr_t o;

	va_start(ap,format);
	o.s = out;
	o.l = 0;
	o.a = size - 1;
	firestring_estr_vsprintf(&o,format,ap);
	va_end(ap);
	firestring_estr_0(&o);
	return o.l;
}

int firestring_strncasecmp(const char * restrict const s1, const char * restrict const s2, const long n) {
	long s;
	for (s = 0; s < n; s++) {
		if (tolower((unsigned char) s1[s]) != tolower((unsigned char) s2[s])) {
			if (tolower((unsigned char) s1[s]) < tolower((unsigned char) s2[s]))
				return -1;
			else
				return 1;
		}
		if (s1[s] == '\0')
			return 0;
	}
	return 0;
}

int firestring_strcasecmp(const char * restrict const s1, const char * restrict const s2) { /* case-insensitive string comparison, ignores tail of longer string */
	size_t s;
	char c1, c2;
	for (s = 0; (s1[s] != '\0') && (s2[s] != '\0') && (tolower((unsigned char) s1[s]) == tolower((unsigned char) s2[s])); s++);

	c1 = tolower((unsigned char) s1[s]);
	c2 = tolower((unsigned char) s2[s]);
	if (c1 == c2)
		return 0;
	else if (c1 < c2)
		return -1;
	else
		return 1;
}

char *firestring_concat (const char * restrict const s, ...) {
	const char *curr;
	size_t len = 0;
	va_list va;
	char * restrict ret = NULL;

	/* get length */
	va_start(va,s);
	curr = s;
	while (curr != NULL) {
		len += strlen(curr);
		curr = va_arg(va, const char *);
	}
	va_end(va);

	/* allocate string */
	if (len) {
		ret = firestring_malloc(len + 1);
		*ret = '\0';
	}

	/* copy */
	va_start(va,s);
	curr = s;
	while (curr != NULL) {
		strcat(ret,curr);
		curr = va_arg(va, const char *);
	}
	va_end(va);

	return ret;
}

char *firestring_chomp(char *s) { /* remove trailing whitespace */
	char *end = s;
	if (end != NULL) {
		end = &s[strlen(s) - 1];
		while (isspace(*end)) {
			*end = '\0';
			if (end == s)
				break;
			end--;
		}
	}
	return s;
}

char *firestring_chug(char * s) { /* ignore frontal whitespace */
	if (s != NULL) {
		while (isspace(*s))
			s++;
	}
	return s;
}

char *firestring_trim(char *s) {
	return firestring_chug(firestring_chomp(s));
}

void firestring_estr_alloc(struct firestring_estr_t * const f, const long a) {
	if (a + 1 > UINT_MAX) {
		errno = EINVAL;
		error_handler();
	}
	f->s = firestring_malloc(a + 1);
	f->a = a;
	f->l = 0;
}

void firestring_estr_expand(struct firestring_estr_t * const f, const long a) {
	if (f->a >= a)
		return;
	if (a + 1 > UINT_MAX) {
		errno = EINVAL;
		error_handler();
	}
	f->s = firestring_realloc(f->s, a + 1);
	f->a = a;
}

void firestring_estr_free(struct firestring_estr_t * const f) {
	if (f->s != NULL)
		free(f->s);
	f->a = 0;
	f->l = 0;
	f->s = NULL;
}

void firestring_estr_0(struct firestring_estr_t * const f) {
	if (f->s != NULL)
		f->s[f->l] = '\0';
}

int firestring_estr_read(struct firestring_estr_t * const f, const int fd) {
	long i;
	if (f->l == f->a)
		return 2;
	i = read(fd,&f->s[f->l],f->a - f->l);
	if (i == -1 && errno == EAGAIN)
		return 0;
	if (i <= 0)
		return 1;
	f->l += i;
	return 0;
}

long firestring_estr_sprintf(struct firestring_estr_t * restrict const o, const char * restrict const format, ...) {
	va_list ap;

	va_start(ap,format);
	return firestring_estr_vsprintfe(o,&ESTR_D((char *)format),ap);
}

long firestring_estr_sprintfe(struct firestring_estr_t * restrict const o, const struct firestring_estr_t * restrict const format, ...) {
	va_list ap;

	va_start(ap,format);
	return firestring_estr_vsprintfe(o,format,ap);
}

long firestring_estr_vsprintf(struct firestring_estr_t * restrict const o, const char * restrict const format, va_list ap) {
	return firestring_estr_vsprintfe(o,&ESTR_D((char *)format),ap);
}

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
				case 'd': /* signed int */
					SIGNED_NUMBER(int)
					break;
				case 'l': /* signed long */
					SIGNED_NUMBER(long)
					break;
				case 'u': /* unsigned int */
					UNSIGNED_NUMBER(unsigned int)
					break;
				case 'y': /* unsigned long */
					UNSIGNED_NUMBER(unsigned long)
					break;
				case 'g': /* signed long long */
					SIGNED_NUMBER(long long)
					break;
				case 'o': /* unsigned long long */
					UNSIGNED_NUMBER(unsigned long long)
					break;
				case 'f': /* double */
					FSIGNED_NUMBER(double);
					break;
				case 't': /* time */
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

long firestring_vfprintf(FILE *stream, const char * restrict const format, va_list ap) {
	return firestring_vfprintfe(stream,&ESTR_D((char *)format),ap);
}

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
				case 'd': /* signed int */
					SIGNED_NUMBER(int)
					break;
				case 'l': /* signed long */
					SIGNED_NUMBER(long)
					break;
				case 'u': /* unsigned int */
					UNSIGNED_NUMBER(unsigned int)
					break;
				case 'y': /* unsigned long */
					UNSIGNED_NUMBER(unsigned long)
					break;
				case 'g': /* signed long long */
					SIGNED_NUMBER(long long)
					break;
				case 'o': /* unsigned long long */
					UNSIGNED_NUMBER(unsigned long long)
					break;
				case 'f': /* double */
					FSIGNED_NUMBER(double);
					break;
				case 't': /* time */
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

long firestring_estr_strchr(const struct firestring_estr_t * const f, const char c, const long start) {
	long i;

	for (i = start; i < f->l; i++)
		if (f->s[i] == c)
			return i;

	return -1;
}

long firestring_estr_strstr(const struct firestring_estr_t * restrict const f, const char * restrict s, const long start) {
	long i;
	long l;
	
	l = strlen(s);
	
	for (i = start; i <= f->l - l; i++)
		if (memcmp(&f->s[i],s,l) == 0)
			return i;

	return -1;
}

long firestring_estr_stristr(const struct firestring_estr_t * restrict const f, const char * restrict s, const long start) {
	long i;
	long j;
	long l;
	
	l = strlen(s);
	
	for (i = start; i <= f->l - l; i++) {
		for (j = 0; j < l; j++)
			if (tolower(f->s[i+j]) != tolower(s[j]))
					break;
		if (j == l)
			return i;
	}

	return -1;
}

int firestring_estr_starts(const struct firestring_estr_t * restrict const f, const char * restrict const s) {
	return firestring_estr_estarts(f,&ESTR_D((char *)s));
}

int firestring_estr_ends(const struct firestring_estr_t * restrict const f, const char * restrict const s) {
	return firestring_estr_eends(f,&ESTR_D((char *)s));
}

int firestring_estr_strcasecmp(const struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long i;

	if (f->l != strlen(s))
		return 1;

	for (i = 0; i < f->l; i++)
		if (tolower(f->s[i]) != tolower(s[i]))
			return 1;

	return 0;
}

int firestring_estr_strcmp(const struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long i;

	if (f->l != strlen(s))
		return 1;

	for (i = 0; i < f->l; i++)
		if (f->s[i] !=s[i])
			return 1;

	return 0;
}

int firestring_estr_strcpy(struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long l;

	l = strlen(s);
	
	if (f->a < l)
		return 1;

	f->l = l;
	memcpy(f->s,s,l);
	return 0;
}

int firestring_estr_astrcpy(struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long l;

	l = strlen(s);

	firestring_estr_expand(f,f->l + l);
	
	f->l = l;
	memcpy(f->s,s,l);
	return 0;
}

int firestring_estr_strcat(struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long l;

	l = strlen(s);
	
	if (l + f->l > f->a)
		return 1;

	memcpy(&f->s[f->l],s,l);
	f->l += l;
	return 0;
}

int firestring_estr_astrcat(struct firestring_estr_t * restrict const f, const char * restrict const s) {
	long l;

	l = strlen(s);
	
	firestring_estr_expand(f,f->l + l);

	memcpy(&f->s[f->l],s,l);
	f->l += l;
	return 0;
}

int firestring_estr_estrcasecmp(const struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	long i;

	if (f->l != t->l - start)
		return 1;
	
	for (i = 0; i < f->l; i++)
		if (tolower(f->s[i]) != tolower(t->s[i + start]))
			return 1;

	return 0;
}

int firestring_estr_estrncasecmp(const struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long length, const long start) {
	long i;

	for (i = 0; i < length; i++)
		if (tolower(f->s[i]) != tolower(t->s[i + start]))
			return 1;

	return 0;
}

int firestring_estr_estrcpy(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	if (f->l - start > t->a)
		return 1;

	t->l = f->l - start;
	memcpy(t->s,&f->s[start],t->l);

	return 0;
}

int firestring_estr_aestrcpy(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	firestring_estr_expand(t,f->l - start);
	return firestring_estr_estrcpy(t,f,start);
}

int firestring_estr_munch(struct firestring_estr_t * const t, const long length) {
	if (length > t->l)
		return 1;

	t->l -= length;
	memmove(t->s,&t->s[length],t->l);

	return 0;
}

void firestring_estr_chomp(struct firestring_estr_t * const s) { /* remove trailing whitespace */
	while (s->l > 0 && isspace(s->s[s->l - 1]))
		s->l--;
}

void firestring_estr_chug(struct firestring_estr_t * const s) { /* remove leading whitespace */
	while (s->l > 0 && isspace(s->s[0])) {
		s->s++;
		s->l--;
		s->a--;
	}
}

void firestring_estr_ip_chug(struct firestring_estr_t * const s) { /* remove leading whitespace in place */
	long start = 0;
	while (start < s->l && isspace(s->s[start]))
		start++;
	memmove(s->s,&s->s[start],s->l - start);
	s->l -= start;
}

void firestring_estr_trim(struct firestring_estr_t * const s) {
	firestring_estr_chug(s);
	firestring_estr_chomp(s);
}

void firestring_estr_ip_trim(struct firestring_estr_t * const s) {
	firestring_estr_ip_chug(s);
	firestring_estr_chomp(s);
}

int firestring_estr_estrcmp(const struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	long i;

	if (f->l != t->l - start)
		return 1;

	for (i = 0; i < f->l; i++)
		if (f->s[i] != t->s[i + start])
			return 1;

	return 0;
}

int firestring_estr_estrcat(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	if (f->l - start + t->l > t->a)
		return 1;

	memcpy(&t->s[t->l],&f->s[start],f->l - start);
	t->l += f->l - start;

	return 0;
}

int firestring_estr_aestrcat(struct firestring_estr_t * restrict const t, const struct firestring_estr_t * restrict const f, const long start) {
	firestring_estr_expand(t,t->l + (f->l - start));
	return firestring_estr_estrcat(t,f,start);
}

long firestring_estr_estrstr(const struct firestring_estr_t * restrict const haystack, const struct firestring_estr_t * restrict const needle, const long start) {
	long i,l;

	l = haystack->l - needle->l;
	for (i = start; i <= l; i++) {
		if (memcmp(&haystack->s[i],needle->s,needle->l) == 0)
				return i;
	}

	return -1;
}

long firestring_estr_estristr(const struct firestring_estr_t * restrict const haystack, const struct firestring_estr_t * restrict const needle, const long start) {
	long i,j,l;

	l = haystack->l - needle->l;
	for (i = start; i <= l; i++) {
		for (j = 0; j < needle->l; j++)
			if (tolower(haystack->s[i+j]) != tolower(needle->s[j]))
					break;
		if (j == needle->l)
			return i;
	}

	return -1;
}

int firestring_estr_estarts(const struct firestring_estr_t * restrict const f, const struct firestring_estr_t * restrict const s) {
	long i;

	if (f->l < s->l)
		return 1;

	for (i = 0; i < s->l; i++)
		if (tolower(f->s[i]) != tolower(s->s[i]))
			return 1;

	return 0;
}

int firestring_estr_eends(const struct firestring_estr_t * restrict const f, const struct firestring_estr_t * restrict const s) {
	long i,t;

	if (f->l < s->l)
		return 1;

	t = f->l - s->l;
	for (i = f->l - 1; i >= t; i--)
		if (tolower(f->s[i]) != tolower(s->s[i - t]))
			return 1;

	return 0;
}

int firestring_estr_replace(struct firestring_estr_t * const dest, const struct firestring_estr_t * const source, const struct firestring_estr_t * const to, const struct firestring_estr_t * const from) {
	int i = 0, oldi = 0;
	struct firestring_estr_t tempe;
	dest->l = 0;
	while ((i = firestring_estr_estrstr(source,from,i)) != -1) {
		tempe.s = &source->s[oldi];
		tempe.l = tempe.a = i - oldi;
		if (firestring_estr_estrcat(dest,&tempe,0) != 0)
			return 1;
		if (firestring_estr_estrcat(dest,to,0) != 0)
			return 1;
		i = oldi = i + from->l;
	}
	tempe.s = &source->s[oldi];
	tempe.l = tempe.a = source->l - oldi;
	if (firestring_estr_estrcat(dest,&tempe,0) != 0)
		return 1;

	return 0;
}

int firestring_estr_areplace(struct firestring_estr_t * const dest, const struct firestring_estr_t * const source, const struct firestring_estr_t * const to, const struct firestring_estr_t * const from) {
	int i = 0, oldi = 0;
	struct firestring_estr_t tempe;
	dest->l = 0;
	while ((i = firestring_estr_estrstr(source,from,i)) != -1) {
		tempe.s = &source->s[oldi];
		tempe.l = tempe.a = i - oldi;
		if (firestring_estr_aestrcat(dest,&tempe,0) != 0)
			return 1;
		if (firestring_estr_aestrcat(dest,to,0) != 0)
			return 1;
		i = oldi = i + from->l;
	}
	tempe.s = &source->s[oldi];
	tempe.l = tempe.a = source->l - oldi;
	if (firestring_estr_aestrcat(dest,&tempe,0) != 0)
		return 1;

	return 0;
}

int firestring_estr_tolower(struct firestring_estr_t * const dest, const struct firestring_estr_t * const source, const long start) {
	long i;

	if (source->l - start > dest->a)
		return 1;

	for (i = start; i < source->l; i++)
		dest->s[i - start] = tolower(source->s[i]);

	return 0;
}

int firestring_estr_toupper(struct firestring_estr_t * const dest, const struct firestring_estr_t * const source, const long start) {
	long i;

	if (source->l - start > dest->a)
		return 1;

	for (i = start; i < source->l; i++)
		dest->s[i - start] = toupper(source->s[i]);

	return 0;
}

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
			/* numeric */
			k = firestring_estr_strchr(f,';',j + 1);
			if (k == -1) /* unterminated entity */
				return 1;
			if (f->s[j + 2] == 'x') {
				/* hex */
				j += 3;
				for (l = j; l < k; l += 2) {
					tempint = firestring_hextoi(&f->s[l]);
					if (tempint == -1) /* invalid hex */
						return 1;
					t->s[o++] = tempint;
				}
			} else {
				/* decimal */
				if (k - j < 3 || k - j > 5)
					return 1; /* too long or too short */
				t->s[o++] = (char) atoi(&f->s[j + 2]);
			}
			i = k + 1;
		} else {
			/* symbolic */
			for (l = 0; xml_decode_table[l].entity != NULL; l++) {
				k = strlen(xml_decode_table[l].entity);
				if (j + l < f->l && memcmp(&f->s[j],xml_decode_table[l].entity,k) == 0)
					break;
			}
			if (xml_decode_table[l].entity == NULL) /* unknown entity */
				return 1;
			t->s[o++] = xml_decode_table[l].character;
			i = j + strlen(xml_decode_table[l].entity);
		}
	}
	memmove(&t->s[o],&f->s[i],f->l - i);
	t->l = o + (f->l - i);

	return 0;
}
 
/* strip quotes, escape chars, and trailing chars from the string. */
static enum firestring_conf_parse_line_result strip_quotes(char **line) {
	char *s, *last_quote = NULL;
	int escaped = 0;

	if (line == NULL || *line == NULL)
		return FSC_MALFORMED;
	
	/* if unquoted, just strip whitespace, and nothing else */
	*line = firestring_chug(firestring_chomp(*line));
	if (**line != '"')
		return FSC_VALID;

	/* if quoted, strip initial quote, locate the trailing quote, and get rid of escape chars */
	*line = *line + 1;
	for (s = *line; *s != '\0'; s++) {
		switch (*s) {
			case '\\':
				escaped = 1;
				memmove(s, s + 1, strlen(s) - 1);
				if (*s == '\0')
					s--;	/* force a break from the for loop */
				else if (*s == '\n') {
					memmove(s, s + 1, strlen(s) - 1);	/* drop newline from string */
					s[strlen(s) - 2] = '\0';
				}
				else if (s[0] == '\r' && s[1] == '\n') {
					memmove(s, s + 2, strlen(s) - 1);	/* or dos-style CRLF crap */
					s[strlen(s) - 3] = '\0';
				}
				else
					s[strlen(s) - 1] = '\0';
				break;
			case '"':
				if (!escaped)
					last_quote = s;
				escaped = 0;
				break;
			default:
				escaped = 0;
		}
	}

	if (last_quote == NULL)
		return FSC_INCOMPLETE;
	*last_quote = '\0';
	return FSC_VALID;
}

/* handle include keyword; the way we detect infinite include loops is to add a conf variable w/ the filename. */
static enum firestring_conf_parse_line_result keyword_include_handler(char *line, struct firestring_conf_t **conf) {
	char *sig;

	line = firestring_chug(firestring_chomp(line));
	sig = firestring_concat("inside_include_handler_", line, NULL);
	if (firestring_conf_find(*conf, sig) == NULL) {
		*conf = firestring_conf_add(*conf, sig, "");
		*conf = firestring_conf_parse_next(line, *conf);
		*conf = firestring_conf_delete(*conf, sig);
	}
	free(sig);

	return FSC_VALID;
}


enum firestring_conf_parse_line_result firestring_conf_parse_line(const char *line, const struct firestring_conf_keyword_t keywords[], struct firestring_conf_t **conf, char **context) {
	enum firestring_conf_parse_line_result err;
	char *curr, *ptr, *var = NULL, *val = NULL;
	int i;

	if (context == NULL || conf == NULL)
		return FSC_MALFORMED;

	/* prepend previous line, if it was incomplete */
	curr = firestring_concat(*context ? *context : "", line, NULL);
	ptr = firestring_chug(curr);

	/* check if the line starts w/ a keyword; if so, call the callback */
	for (i=0; keywords[i].keyword != NULL; i++) {
		if (strncmp(keywords[i].keyword, ptr, strlen(keywords[i].keyword)) == 0) {
			if (keywords[i].callback != NULL)
				err = keywords[i].callback(ptr + strlen(keywords[i].keyword), conf);
			else
				err = FSC_VALID;
			goto done;
		}
	}

	/* not a keyword; must be an assignment.  split apart. */
	val = strchr(ptr, '=');
	if (val == NULL) {
		err = FSC_MALFORMED;
		goto done;
	}
	*val++ = '\0';
	var = firestring_chomp(ptr);
	
	/* and finally, strip quotes */
	err = strip_quotes(&val);

done:
	if (err == FSC_INCOMPLETE) {
		ptr = *context;
		*context = firestring_concat(*context ? *context : "", line, NULL);
		if (ptr)
			free(ptr);
	}
	else if (*context != NULL) {
		free(*context);
		*context = NULL;
	}
	if (err == FSC_VALID) {
		*conf = firestring_conf_add(*conf, var, val);
	}
	free(curr);
	return err;
}

static struct firestring_conf_keyword_t default_keywords[] = {
	{ "include ", &keyword_include_handler },
	{ "#", NULL },	/* ignore comments */
	{ NULL, NULL },
};

struct firestring_conf_t *firestring_conf_parse_next(const char * const filename, struct firestring_conf_t *prev) {
	struct firestring_conf_t *conf = prev;
	char buf[512], *ctx = NULL;
	FILE *fp;

	fp = fopen(filename, "r");
	if (fp != NULL) {
		while (fgets(buf, sizeof(buf), fp) != NULL) {
			firestring_conf_parse_line(buf, default_keywords, &conf, &ctx);
		}
		fclose(fp);
	}

	return conf;
}

struct firestring_conf_t *firestring_conf_parse(const char * const filename) {
	return firestring_conf_parse_next(filename, NULL);
}

struct firestring_conf_t *firestring_conf_add(struct firestring_conf_t * restrict const next, const char * restrict const var, const char * restrict const value) { /* insert config val in list, return new list head */
	struct firestring_conf_t *conf;

	if (var != NULL && value != NULL) {
		conf = firestring_malloc(sizeof(*conf));
		conf->next = next;
		conf->var = firestring_strdup(var);
		conf->value = firestring_strdup(value);
	} else
		conf = next;

	return conf;
}

struct firestring_conf_t *firestring_conf_delete(struct firestring_conf_t *conf, const char *var) {
	struct firestring_conf_t *prev = NULL, *head = conf;
	
	while (conf != NULL) {
		if (firestring_strcasecmp(conf->var, var) == 0) {
			free(conf->var);
			free(conf->value);
			if (prev)
				prev->next = conf->next;
			if (head == conf)
				head = conf->next;
			free(conf);
			break;
		}
		prev = conf;
		conf = conf->next;
	}

	return head;
}

char *firestring_conf_find(const struct firestring_conf_t * restrict config, const char * restrict const var) { /* find in list and return pointer to config val */
	return firestring_conf_find_next(config,var,NULL);
}

char *firestring_conf_find_next(const struct firestring_conf_t * restrict config, const char * restrict const var, const char * const prev) {
	int canreturn = 0;

	if (prev == NULL)
		canreturn = 1;

	while (config != NULL) {
		if (firestring_strcasecmp(config->var, var) == 0) {
			if (canreturn == 1)
				return config->value;
			else if (config->value == prev)
				canreturn = 1;
		}
		config = config->next;
	}

	return NULL;
}

void firestring_conf_free(struct firestring_conf_t * config) {
	struct firestring_conf_t *next;

	while (config != NULL) {
		next = config->next;
		free(config->var);
		free(config->value);
		free(config);
		config = next;
	}
}
