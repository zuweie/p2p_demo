#ifndef _MSG_H_
#define _MSG_H_
#include "_endpoint.h"

typedef enum _Msg {
    EMSG_PUNCH = 1,
    EMSG_BEATING,
    EMSG_PEERLIST,
    EMSG_ONPEERLIST,
    EMSG_ONCHAT
}Msg; 

extern int send_udp_msg(int socketfd, Endpoint *to, const char * msg);

#endif