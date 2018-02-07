#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "_endpoint.h"
#include "_logging.h"

#define MSGBUF_SIZE 129

int g_socketfd;
Endpoint g_server;

// 此函数不断给服务器发送心跳包
void keeplive_loop();

int main (int argc, char** argv) 
{
    log_setlevel(INFO);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s server:port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    ep_fromstring(&g_server, argv[1]);

    g_socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_socketfd == -1){
        close(g_socketfd);
        fprintf(stderr, "socket faild");
        exit(EXIT_FAILURE);
    }

    pthread_t keepalive_pid;
    int ret = pthread_create(&keepalive_pid, NULL, &keeplive_loop, NULL);
    if (ret != 0) {
        perror("keepalive");
        goto clean;
    }
    pthread_join(keepalive_pid, NULL);
clean:
    return EXIT_FAILURE;
}

void keeplive_loop() {
    char msg[MSGBUF_SIZE];
    int i=0;
    for(;;){
        sleep(1);
        log_info("send a message to server\n");
        memset(msg, 0, MSGBUF_SIZE);
        snprintf(msg, MSGBUF_SIZE, "hello server I am %d\n", ++i);
        sendto(g_socketfd, msg, MSGBUF_SIZE, MSG_DONTWAIT, (struct sockaddr*)&g_server, sizeof(g_server));
    }
}