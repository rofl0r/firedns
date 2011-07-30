#include "firedns_internal.h"

struct firedns_mxlist *firedns_resolvemxalist(const char * const name) {
	int t,i,n,c = 0;
	int cname_fd[256] = {0};
	int alist_fd[256] = {0};
	int a6list_fd[256] = {0};
	void *ret;
	struct firedns_mxlist *mxlist, *iter;
	fd_set s;
	struct timeval tv;
	int firstround = 1;
	mxlist = firedns_resolvemxlist_r(name);
	if (mxlist == NULL) {
		mxlist = firestring_malloc(sizeof(struct firedns_mxlist) + strlen(name) + 1 + FIREDNS_ALIGN);
		mxlist->next = NULL;
		mxlist->cname = NULL;
		mxlist->ip4list = NULL;
		mxlist->ip6list = NULL;
		mxlist->protocol = FIREDNS_MX_SMTP;
		mxlist->priority = 0;
		mxlist->name = firedns_align(((char *)mxlist) + sizeof(struct firedns_mxlist));
		strcpy(mxlist->name,name);
	}
	
	iter = mxlist;
	while (iter != NULL) {
		iter->ip4list = firestring_malloc(RESULTSIZE);
		iter->ip6list = firestring_malloc(RESULTSIZE);
		iter->cname = firestring_malloc(RESULTSIZE);
		iter = iter->next;
		c += 3;
	}
	
	for (t = 0; t < FIREDNS_TRIES; t++) {
		iter = mxlist;
		n = i = 0;
		FD_ZERO(&s);
		while (iter != NULL) {
			if (cname_fd[i] != -1) {
				if (!firstround)
					(void) firedns_getresult(cname_fd[i]);
				cname_fd[i] = firedns_getcname(iter->name);
				if (cname_fd[i] == -1) {
					firedns_free_mxalist(mxlist);
					return NULL;
				}
				FD_SET(cname_fd[i],&s);
				n = max(n,cname_fd[i]);
			}
			if (alist_fd[i] != -1) {
				if (!firstround)
					(void) firedns_getresult(alist_fd[i]);
				alist_fd[i] = firedns_getip4list(iter->name);
				if (alist_fd[i] == -1) {
					firedns_free_mxalist(mxlist);
					return NULL;
				}
				FD_SET(alist_fd[i],&s);
				n = max(n,alist_fd[i]);
			}
			if (a6list_fd[i] != -1) {
				if (!firstround)
					(void) firedns_getresult(a6list_fd[i]);
				a6list_fd[i] = firedns_getip6list(iter->name);
				if (a6list_fd[i] == -1) {
					firedns_free_mxalist(mxlist);
					return NULL;
				}
				FD_SET(a6list_fd[i],&s);
				n = max(n,a6list_fd[i]);
			}
			i++;
			iter = iter->next;
		}
		firstround = 0;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		i = select(n + 1,&s,NULL,NULL,&tv);
		
		if (i == 0)
			continue;
		else
			t--;
		iter = mxlist;
		i = 0;
		while (iter != NULL) {
			if (cname_fd[i] != -1 && FD_ISSET(cname_fd[i],&s)) {
				ret = firedns_getresult_s(cname_fd[i],iter->cname);
				if (ret == NULL) {
					free(iter->cname);
					iter->cname = NULL;
				}
				cname_fd[i] = -1;
				c--;
			}
			if (alist_fd[i] != -1 && FD_ISSET(alist_fd[i],&s)) {
				ret = firedns_getresult_s(alist_fd[i],(char *)iter->ip4list);
				if (ret == NULL) {
					free(iter->ip4list);
					iter->ip4list = NULL;
				}
				alist_fd[i] = -1;
				c--;
			}
			if (a6list_fd[i] != -1 && FD_ISSET(a6list_fd[i],&s)) {
				ret = firedns_getresult_s(a6list_fd[i],(char *)iter->ip6list);
				if (ret == NULL) {
					free(iter->ip6list);
					iter->ip6list = NULL;
				}
				a6list_fd[i] = -1;
				c--;
			}
			i++;
			iter = iter->next;
		}
		if (c == 0)
			return mxlist;
	}
	iter = mxlist;
	i = 0;
	while (iter != NULL && c > 0) {
		if (cname_fd[i] != -1) {
			(void) firedns_getresult(cname_fd[i]);
			free(iter->cname);
			iter->cname = NULL;
			cname_fd[i] = -1;
			c--;
		}
		if (alist_fd[i] != -1) {
			(void) firedns_getresult(alist_fd[i]);
			free(iter->ip4list);
			iter->ip4list = NULL;
			alist_fd[i] = -1;
			c--;
		}
		if (a6list_fd[i] != -1) {
			(void) firedns_getresult(a6list_fd[i]);
			free(iter->ip6list);
			iter->ip6list = NULL;
			a6list_fd[i] = -1;
			c--;
		}
		i++;
		iter = iter->next;
	}
	free(mxlist);
	return NULL;
}

