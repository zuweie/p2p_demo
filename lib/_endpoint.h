#ifndef _ENDPOINT_H_
#define _ENDPOINT_H_
#include <netinet/in.h>

typedef struct sockaddr_in Endpoint;
extern int ep_frompair(Endpoint* ep, const char* host, short port);
extern const char* ep_tostring(Endpoint* ep);
extern int ep_fromstring(Endpoint* ep, const char*);
#endif
