#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include "../../include/firestring.h"

#define _FIRESTRING_C
#define max(a,b) (a > b ? a : b)
#define min(a,b) (a < b ? a : b)
struct xml_encoding {
	char character;
	char *entity;
};

#define XML_UNSAFE "<>\"&"
#define XML_WORSTCASE 6


extern void firestring_int_errorhandler();
extern void (*error_handler)() ;
extern const char base64_encode_table[] ;
extern const char base64_decode_table[] ;
extern const char hex_decode_table[] ;
extern const struct xml_encoding xml_decode_table[] ;
extern struct firestring_conf_keyword_t default_keywords[] ;
void firestring_int_errorhandler();
int showdate(char * const dest, const int space, const time_t t);
int shownum_unsigned(unsigned long long m, const int padzero, int numpad, char * const numbuf, const int space);
int shownum_funsigned(const double m, const int padzero, int numpad, char * const numbuf, const int space);
int shownum_signed(const long long n, const int padzero, const int numpad, char * const numbuf, const int space);
int shownum_fsigned(const double n, const int padzero, const int numpad, char * const numbuf, const int space);
enum firestring_conf_parse_line_result strip_quotes(char **line);
enum firestring_conf_parse_line_result keyword_include_handler(char *line, struct firestring_conf_t **conf);

//RcB: DEP "*.c"

