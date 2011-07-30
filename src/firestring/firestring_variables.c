#include "firestring_internal.h"

void firestring_int_errorhandler();

void (*error_handler)() = firestring_int_errorhandler;

