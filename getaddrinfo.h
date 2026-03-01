#ifndef EIO_GETADDRINFO_
#define EIO_GETADDRINFO_

#include <netdb.h>
#include "eio.h"

eio_req* eio_getaddrinfo(const char *hostname, const char *servname,
                         const struct addrinfo *hints,
                         int pri, eio_cb cb, void *data);

#endif
