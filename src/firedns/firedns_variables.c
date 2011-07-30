#include "firedns_internal.h"

const int firedns_mx_port[] = { 25, 209, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0 };

const char *firedns_mx_name[] = { "SMTP", "QMTP", "Unknown", "Unknown",
					"Unknown", "Unknown", "Unknown", "Unknown",
					"Unknown", "Unknown", "Unknown", "Unknown",
					"Unknown", "Unknown", "Unknown", "Unknown" };

struct in_addr servers4[FDNS_MAX]; 

int i4; 

#ifdef HAVE_IPV6
int i6;
struct in6_addr servers6[FDNS_MAX];
#endif

int initdone = 0; 

int wantclose = 0;

int lastcreate = -1;

pthread_mutex_t connlist_lock = PTHREAD_MUTEX_INITIALIZER;

struct s_connection *connection_head = NULL; 

