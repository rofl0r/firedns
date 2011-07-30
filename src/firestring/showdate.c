#include "firestring_internal.h"

int showdate(char * const dest, const int space, const time_t t) {
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

