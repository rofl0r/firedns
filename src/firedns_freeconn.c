#include "firedns_internal.h"
#include <string.h>

void firedns_freeconn(firedns_state* self, struct s_connection* conn) {
	size_t index = (conn - &self->connections[0]) / 
		(&self->connections[1] - &self->connections[0]);
		// ^ use the exact distance including padding...
	self->conn_used[index] = 0;
}
