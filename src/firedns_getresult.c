#include "firedns_internal.h"
#include <string.h>

char *firedns_getresult(firedns_state* self, const int fd) { 
	char* result = self->resultbuf;
	struct s_header h;
	struct s_connection * restrict c, *prev;
	int i, l, o;
	unsigned int q, curanswer;
	struct s_rr_middle rr;
	unsigned char buffer[sizeof(struct s_header)];
	unsigned short p;
	prev = NULL;
	c = self->connection_head;
	while (c != NULL) { 
		if (c->fd == fd)
			break;
		prev = c;
		c = c->next;
	}
	if (c == NULL) {
		return NULL; 
	}
	
	if (prev != NULL)
		prev->next = c->next;
	else
		self->connection_head = c->next;
	l = recv(c->fd, buffer, sizeof(struct s_header), 0);
	firedns_close(self, c->fd);
	if (l < 12)
		goto error_handler;
	firedns_fill_header(&h,buffer,l - 12);
	if (c->id[0] != h.id[0] || c->id[1] != h.id[1])
		goto error_handler;
	
	if ((h.flags1 & FLAGS1_MASK_QR) == 0)
		goto error_handler;
	
	if ((h.flags1 & FLAGS1_MASK_OPCODE) != 0)
		goto error_handler;
	
	if ((h.flags2 & FLAGS2_MASK_RCODE) != 0) 
		goto error_handler;
	
	if (h.ancount < 1)  
		goto error_handler;
	
	i = 0;
	q = 0;
	l -= 12;
	while (q < h.qdcount && i < l) {
		if (h.payload[i] > 63) { 
			i += 6; 
			q++;
		} else { 
			if (h.payload[i] == 0) {
				q++;
				i += 5; 
			} else
				i += h.payload[i] + 1; 
		}
	}
	
	curanswer = 0;
	while (curanswer < h.ancount) {
		q = 0;
		while (q == 0 && i < l) {
			if (h.payload[i] > 63) { 
				i += 2; 
				q = 1;
			} else { 
				if (h.payload[i] == 0) {
					i++;
					q = 1;
				} else
					i += h.payload[i] + 1; 
			}
		}
		if (l - i < 10)
			goto error_handler;
		
		firedns_fill_rr(&rr,&h.payload[i]);
		i += 10;
		if (rr.type != c->type) {
			curanswer++;
			i += rr.rdlength;
			continue;
		}
		if (rr.dclass != c->dclass) {
			curanswer++;
			i += rr.rdlength;
			continue;
		}
		break;
	}
	// XXX these 3 did not free the conn!
	if (curanswer == h.ancount)
		goto error_handler;

	if (i + rr.rdlength > (unsigned) l)
		goto error_handler;

	if (rr.rdlength > 1023)
		goto error_handler;

	switch (rr.type) {
		case FDNS_QRY_PTR:
		case FDNS_QRY_CNAME:
			o = 0;
			q = 0;
			while (q == 0 && i < l && o + 256 < 1023) {
				if (h.payload[i] > 63) { 
					memcpy(&p,&h.payload[i],2);
					i = ntohs(p) - FIREDNS_POINTER_VALUE - 12;
				} else { 
					if (h.payload[i] == 0)
						q = 1;
					else {
						result[o] = '\0';
						if (o != 0)
							result[o++] = '.';
						memcpy(&result[o],&h.payload[i + 1],h.payload[i]);
						o += h.payload[i];
						i += h.payload[i] + 1;
					}
				}
			}
			result[o] = '\0';
			break;
		case FDNS_QRY_MX:
			if (c->want_list) {
				struct firedns_mxlist *mxa = (struct firedns_mxlist *) result; 
				int g;
				while ((char *)mxa - (char *)result < 700) {
					mxa->ip4list = NULL;
					mxa->cname = NULL;
					g = i + rr.rdlength;
					curanswer++;
					mxa->priority = h.payload[i] * 256 + h.payload[i+1];
					if (mxa->priority >= FDNS_MXPS_START && mxa->priority <= FDNS_MXPS_STOP)
						mxa->protocol = mxa->priority % 16;
					else
						mxa->protocol = FIREDNS_MX_SMTP;
					mxa->name = (char *)mxa + sizeof(struct firedns_mxlist);
					i += 2;
					o = 0;
					q = 0;
					while (q == 0 && i < l && o + 256 < 1023) {
						if (h.payload[i] > 63) { 
							memcpy(&p,&h.payload[i],2);
							i = ntohs(p) - FIREDNS_POINTER_VALUE - 12;
						} else { 
							if (h.payload[i] == 0)
								q = 1;
							else {
								mxa->name[o] = '\0';
								if (o != 0)
									mxa->name[o++] = '.';
								memcpy(&mxa->name[o],&h.payload[i + 1],h.payload[i]);
								o += h.payload[i];
								i += h.payload[i] + 1;
							}
						}
					}
					mxa->name[o++] = '\0';
					mxa->next = NULL;
					i = g;
					if (curanswer < h.ancount) {
						q = 0;
						while (q == 0 && i < l) {
							if (h.payload[i] > 63) { 
								i += 2; 
								q = 1;
							} else { 
								if (h.payload[i] == 0) {
									i++;
									q = 1;
								} else
									i += h.payload[i] + 1; 
							}
						}
						if (l - i < 10) {
							goto error_handler;
						}
						firedns_fill_rr(&rr,&h.payload[i]);
						i += 10;
						if (rr.type != FDNS_QRY_MX)
							break;
						if (rr.dclass != 1)
							break;
						mxa->next = (struct firedns_mxlist *) firedns_align((((char *) mxa) + sizeof(struct firedns_mxlist) + o));
						mxa = mxa->next;
					} else {
						break;
					}
				}
				mxa->next = NULL;
			} else {
				i += 2;
				o = 0;
				q = 0;
				while (q == 0 && i < l && o + 256 < 1023) {
					if (h.payload[i] > 63) { 
						memcpy(&p,&h.payload[i],2);
						i = ntohs(p) - FIREDNS_POINTER_VALUE - 12;
					} else { 
						if (h.payload[i] == 0)
							q = 1;
						else {
							result[o] = '\0';
							if (o != 0)
								result[o++] = '.';
							memcpy(&result[o],&h.payload[i + 1],h.payload[i]);
							o += h.payload[i];
							i += h.payload[i] + 1;
						}
					}
				}
				result[o] = '\0';
			}
			break;
		case FDNS_QRY_TXT:
			if (c->want_list) {
				struct firedns_txtlist *txtlist = (struct firedns_txtlist *) result; 
				while ((char *)txtlist - (char *)result < 700) {
					if (rr.type != FDNS_QRY_TXT)
						break;
					if (rr.dclass != 1)
						break;
					{
						unsigned char *end, *trailer;
						int o;
						txtlist->txt = firedns_align(((char *)txtlist) + sizeof(struct firedns_txtlist));
						trailer = &h.payload[i];
						end = &h.payload[i] + rr.rdlength;
						o = 0;
						while (trailer < end) {
							unsigned char l;
							l = trailer[0];
							if (trailer + l > end) { 
								goto error_handler;
							}
							memcpy(&txtlist->txt[o],&trailer[1],l);
							o += l; 
							trailer += l + 1;
						}
						txtlist->txt[o] = '\0';
					}
					if (++curanswer >= h.ancount)
						break;
					i += rr.rdlength;
					{
						
						q = 0;
						while (q == 0 && i < l) {
							if (h.payload[i] > 63) { 
								i += 2; 
								q = 1;
							} else { 
								if (h.payload[i] == 0) {
									i++;
									q = 1;
								} else
									i += h.payload[i] + 1; 
							}
						}
					}
					if (l - i < 10) {
						goto error_handler;
					}
					firedns_fill_rr(&rr,&h.payload[i]);
					i += 10;
					txtlist->next = (struct firedns_txtlist *) firedns_align(txtlist->txt + strlen(txtlist->txt) + 1);
					txtlist = txtlist->next;
					txtlist->next = NULL;
				}
				txtlist->next = NULL;
				break;
			} else {
				unsigned char *end, *trailer;
				int o = 0;
				trailer = &h.payload[i];
				end = &h.payload[i] + rr.rdlength;
				while (trailer < end) {
					unsigned char l = trailer[0];
					if (trailer + l > end) { 
						goto error_handler;
					}
					memcpy(&result[o],&trailer[1],l);
					o += l; 
					trailer += l + 1;
				}
				result[o] = '\0';
			}
			break;
		case FDNS_QRY_A:
			if (c->want_list) {
				struct firedns_ip4list *alist = (struct firedns_ip4list *) result; 
				while ((char *)alist - (char *)result < 700) {
					if (rr.type != FDNS_QRY_A)
						break;
					if (rr.dclass != 1)
						break;
					if (rr.rdlength != 4) {
						goto error_handler;
					}
					memcpy(&alist->ip,&h.payload[i],4);
					if (++curanswer >= h.ancount)
						break;
					i += rr.rdlength;
					{
						
						q = 0;
						while (q == 0 && i < l) {
							if (h.payload[i] > 63) { 
								i += 2; 
								q = 1;
							} else { 
								if (h.payload[i] == 0) {
									i++;
									q = 1;
								} else
									i += h.payload[i] + 1; 
							}
						}
					}
					if (l - i < 10) {
						goto error_handler;
					}
					firedns_fill_rr(&rr,&h.payload[i]);
					i += 10;
					alist->next = (struct firedns_ip4list *) firedns_align(((char *) alist) + sizeof(struct firedns_ip4list));
					alist = alist->next;
					alist->next = NULL;
				}
				alist->next = NULL;
				break;
			}
			goto defaultcase;
			break;
		case FDNS_QRY_AAAA:
			if (c->want_list) {
				struct firedns_ip6list *alist = (struct firedns_ip6list *) result; 
				while ((char *)alist - (char *)result < 700) {
					if (rr.type != FDNS_QRY_AAAA)
						break;
					if (rr.dclass != 1)
						break;
					if (rr.rdlength != 16) {
						goto error_handler;
					}
					memcpy(&alist->ip,&h.payload[i],16);
					if (++curanswer >= h.ancount)
						break;
					i += rr.rdlength;
					{
						
						q = 0;
						while (q == 0 && i < l) {
							if (h.payload[i] > 63) { 
								i += 2; 
								q = 1;
							} else { 
								if (h.payload[i] == 0) {
									i++;
									q = 1;
								} else
									i += h.payload[i] + 1; 
							}
						}
					}
					if (l - i < 10) {
						goto error_handler;
					}
					firedns_fill_rr(&rr,&h.payload[i]);
					i += 10;
					alist->next = (struct firedns_ip6list *) firedns_align(((char *) alist) + sizeof(struct firedns_ip6list));
					alist = alist->next;
					alist->next = NULL;
				}
				alist->next = NULL;
				break;
			}
			goto defaultcase;
			break;
		default:
		defaultcase:
			memcpy(result,&h.payload[i],rr.rdlength);
			result[rr.rdlength] = '\0';
			break;
	}
	firedns_freeconn(self, c);
	return result;
	error_handler:
	firedns_freeconn(self, c);
	return NULL;
}

