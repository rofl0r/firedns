#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include "../../include/firestring.h"
#include "../../include/firedns.h"

#define _FIREDNS_C
#define max(a,b) (a > b ? a : b)
#define FDNS_MAX              8                    
#define FDNS_CONFIG_FBCK     "/etc/resolv.conf"    
#define FDNS_PORT            53                    
#define FDNS_QRY_A            1                    
#define FDNS_QRY_AAAA        28                    
#define FDNS_QRY_PTR         12                    
#define FDNS_QRY_MX          15                    
#define FDNS_QRY_TXT         16                    
#define FDNS_QRY_CNAME       5
#define FDNS_MXPS_START 12800
#define FDNS_MXPS_STOP  13055
#define FIREDNS_ALIGN (sizeof(void *) > sizeof(long) ? sizeof(void *) : sizeof(long))
#define FIREDNS_TRIES 3
#define RESULTSIZE 1024
#define min(a,b) (a < b ? a : b)

struct s_connection { 
	struct s_connection *next; 
	unsigned char id[2]; 
	unsigned int class;
	unsigned int type;
	int want_list;
	int fd; 
#ifdef HAVE_IPV6
	int v6;
#endif
};

struct s_rr_middle {
	unsigned int type;
	unsigned int class;
	unsigned long ttl;
	unsigned int rdlength;
};

#define FIREDNS_POINTER_VALUE 0xc000
struct s_header { 
	unsigned char id[2];
	unsigned int flags1;
#define FLAGS1_MASK_QR 0x80
#define FLAGS1_MASK_OPCODE 0x78 
#define FLAGS1_MASK_AA 0x04
#define FLAGS1_MASK_TC 0x02
#define FLAGS1_MASK_RD 0x01
	unsigned int flags2;
#define FLAGS2_MASK_RA 0x80
#define FLAGS2_MASK_Z  0x70
#define FLAGS2_MASK_RCODE 0x0f
	unsigned int qdcount;
	unsigned int ancount;
	unsigned int nscount;
	unsigned int arcount;
	unsigned char payload[512]; 
};

extern const char tagstring[] ;
extern const char firedns_version[] ;
extern const int firedns_mx_port[] ;
extern const char *firedns_mx_name[] ;
extern struct in_addr servers4[FDNS_MAX]; 

extern int i4; 
#ifdef HAVE_IPV6
extern int i6;
extern struct in6_addr servers6[FDNS_MAX];
#endif
extern int initdone ;
extern int wantclose ;
extern int lastcreate ;
extern pthread_mutex_t connlist_lock ;
extern struct s_connection *connection_head ;
void *firedns_align(void *inp);
void firedns_fill_rr(struct s_rr_middle * restrict const rr, const unsigned char * const restrict input);
void firedns_fill_header(struct s_header * const restrict header, const unsigned char * const restrict input, const int l);
void firedns_empty_header(unsigned char * const restrict output, const struct s_header * const restrict header, const int l);
void firedns_close(int fd);
int firedns_send_requests(const struct s_header * restrict const h, const struct s_connection * restrict const s, const int l);
struct s_connection *firedns_add_query(struct s_header * restrict const h);
int firedns_build_query_payload(const char * const name, const unsigned short rr, const unsigned short class, unsigned char * const payload);
struct in_addr *firedns_resolveip4_i(const char * restrict const name, char *(* const result)(int));
struct firedns_ip4list *firedns_resolveip4list_i(const char * restrict const name, char *(* const result)(int));
struct in6_addr *firedns_resolveip6_i(const char * restrict const name, char *(* const result)(int));
struct firedns_ip6list *firedns_resolveip6list_i(const char * restrict const name, char *(* const result)(int));
char *firedns_resolvetxt_i(const char * restrict const name, char *(* const result)(int));
struct firedns_txtlist *firedns_resolvetxtlist_i(const char * restrict const name, char *(* const result)(int));
char *firedns_resolvemx_i(const char * restrict const name, char *(* const result)(int));
struct firedns_mxlist *firedns_resolvemxlist_i(const char * restrict const name, char *(* const result)(int));
char *firedns_resolvename4_i(const struct in_addr * restrict const ip, char *(* const result)(int));
char *firedns_resolvename6_i(const struct in6_addr * restrict const ip, char *(* const result)(int));
char *firedns_resolvecname_i(const char * restrict const name, char *(* const result)(int));

//RcB: DEP "*.c"

