#include "firedns_internal.h"

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

