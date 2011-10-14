#include "firedns_internal.h"
#include <string.h>

struct s_connection* firedns_getconn(firedns_state* self) {
	struct s_connection* result = NULL;
	size_t i;
	for(i = 0; i < FDNS_MAX; i++) {
		if(!self->conn_used[i]) {
			result = &self->connections[i];
			memset(result, 0, sizeof(struct s_connection));
			self->conn_used[i] = 1;
			break;
		}
	}
	return result;
}
