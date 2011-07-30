#include "firestring_internal.h"

char *firestring_trim(char *s) {
	return firestring_chug(firestring_chomp(s));
}

