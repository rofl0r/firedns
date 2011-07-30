#include "firestring_internal.h"

struct firestring_conf_keyword_t default_keywords[] = {
	{ "include ", &keyword_include_handler },
	{ "#", NULL },
	{ NULL, NULL },
};

