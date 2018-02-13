#include "_msg.h"

#define SENDBUF_SIZE 1024

extern int send_udp_msg(int socketfd, Endpoint *to, const char * msg) {

    char msg_buff[SENDBUF_SIZE];

    memset(msg_buff, 0, SENDBUF_SIZE);

    snprintf(msg_buff, SENDBUF_SIZE, msg);

    sendto(socketfd, msg_buff, SENDBUF_SIZE, MSG_DONTWAIT, to, sizeof(Endpoint));

    return 0;
} 