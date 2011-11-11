#include <sys/types.h>
#include <unistd.h>
#include "../include/firedns.h"

#define _FIREDNS_C
#define max(a,b) (a > b ? a : b)
                  
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

struct s_rr_middle {
	unsigned int type;
	unsigned int dclass;
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

void *firedns_align(void *inp);
void firedns_fill_rr(struct s_rr_middle *  const rr, const unsigned char * const  input);
void firedns_fill_header(struct s_header * const  header, const unsigned char * const  input, const int l);
void firedns_empty_header(unsigned char * const  output, const struct s_header * const  header, const int l);
void firedns_close(firedns_state* self, int fd);
int firedns_send_requests(firedns_state* self, const struct s_header *  const h, const struct s_connection *  const s, const int l);
struct s_connection *firedns_add_query(firedns_state* self, struct s_header *  const h);
int firedns_build_query_payload(const char* name, const unsigned short rr, const unsigned short dclass, unsigned char* payload);

//RcB: DEP "*.c"

