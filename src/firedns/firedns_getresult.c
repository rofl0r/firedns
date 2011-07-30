#include "firedns_internal.h"

char *firedns_getresult(const int fd) { 
	static char result[RESULTSIZE];
	return firedns_getresult_s(fd,result);
}

