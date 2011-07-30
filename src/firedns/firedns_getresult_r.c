#include "firedns_internal.h"

char *firedns_getresult_r(const int fd) { 
	char *result;
	result = firestring_malloc(RESULTSIZE);
	if(firedns_getresult_s(fd,result) == NULL) {
		free(result);
		return NULL;
	}
	return result;
}

