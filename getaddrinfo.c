#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include "eio.h"

static void eio__getaddrinfo(eio_req* req) {
    req->result = getaddrinfo(req->ptr1, (char*)req->ptr1 + req->int2,
                              (struct addrinfo*)((char*)req->ptr1 + req->int3),
                              (struct addrinfo**)&req->ptr2);
}

static void eio__gai_destroy(eio_req* req) {
    freeaddrinfo((struct addrinfo *)req->ptr2);
    free(req);
}

eio_req* eio_getaddrinfo(const char *hostname, const char *servname,
                         const struct addrinfo *hints,
                         int pri, eio_cb cb, void *data) {
    eio_req *req;
    if (!((uintptr_t)hostname | (uintptr_t)servname) ||
        !(req = (eio_req *)calloc (1, sizeof *req)))
        return NULL;

    req->type    = EIO_CUSTOM;
    req->pri     = pri;
    req->finish  = cb;
    req->data    = data;
    req->destroy = eio__gai_destroy;
    req->feed    = eio__getaddrinfo;
    req->flags   = EIO_FLAG_PTR1_FREE;

    req->int2    = hostname ? strlen(hostname) + 1 : 0;
    req->int3    = req->int2 + (servname ? strlen(servname) + 1 : 0);
    req->ptr1    = malloc(req->int3 + sizeof(struct addrinfo));
    if (hostname)
        memcpy(req->ptr1, hostname, req->int2);
    if (servname)
        memcpy((char*)req->ptr1 + req->int2, servname, req->int3 - req->int2 + 1);
    if (hints)
        memcpy((char*)req->ptr1 + req->int3, hints, sizeof(struct addrinfo));

    eio_submit(req);
    return req;
}
