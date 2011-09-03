#include "../../include/firedns.h"
#include <string.h>

void firedns_freeconn(firedns_state* self, struct s_connection* conn) {
	size_t index = (conn - &self->connections[0]) / sizeof(struct s_connection);
	self->conn_used[index] = 0;
}
