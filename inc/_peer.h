#ifndef _PEER_H_
#define _PEER_H_

#include "_endpoint.h"

#define PEERIDSIZE 128

typedef struct _Peer {

    char id[PEERIDSIZE];
    Endpoint io;

}Peer;

#endif 