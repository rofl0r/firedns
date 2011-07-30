#include "firestring_internal.h"

long firestring_vfprintf(FILE *stream, const char * restrict const format, va_list ap) {
	return firestring_vfprintfe(stream,&ESTR_D((char *)format),ap);
}

