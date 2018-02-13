#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "_endpoint.h"
#include "_logging.h"
#include "cJSON.h"
#include "_msg.h"
#include "_msghandler.h"
#include "Const_str.h"

#define MSGBUF_SIZE 129
#define PEER_RECV_BUF 1024

static int g_clientfd;
static Endpoint g_server;
static RBTree g_msg_handler_mapping;

// 此函数不断给服务器发送心跳包
void* keepalive_loop();
void* receive_loop();
void* console_loop();
void send_beating_msg();
void send_query_peerlist_msg();
void process_msg(Endpoint* ep, const char * buf);

static int on_peerlist (Endpoint* ep, void * params) 
{
    
}
 

int main (int argc, char** argv) 
{
    log_setlevel(INFO);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s server:port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    ep_fromstring(&g_server, argv[1]);

    /* 1 init socket*/
    g_clientfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_clientfd == -1){
        close(g_clientfd);
        fprintf(stderr, "socket faild");
        exit(EXIT_FAILURE);
    }
    /* 2 init handler mapping*/
    init_mhmap(&g_msg_handler_mapping);
    add_handler(&g_msg_handler_mapping, EMSG_ONPEERLIST, &on_peerlist);

    pthread_t keepalive_pid, console_pid, receive_pid;
    int ret = pthread_create(&keepalive_pid, NULL, &keepalive_loop, NULL);
    if (ret != 0) {
        perror("keepalive");
        goto clean;
    }

    ret = pthread_create(&console_pid, NULL, &console_loop, NULL);
    if (ret != 0) {
        perror("console");
        goto clean;
    }
    
    ret = pthread_create(&receive_pid, NULL, &receive_loop, NULL);
    if (ret != 0) {
        perror("receive");
        goto clean;
    }
    pthread_join(keepalive_pid, NULL);
    pthread_join(console_pid, NULL);
    pthread_join(receive_pid, NULL);
clean:
    return EXIT_FAILURE;
}

void* keepalive_loop() {

    for(;;){
        sleep(6);
        //log_info("send a message to server\n");
        send_beating_msg();
    }

    return NULL;
}

void* receive_loop() 
{
    Endpoint peer;
    socklen_t addrlen;
    char buf[PEER_RECV_BUF];
    fd_set readfds;
    int maxfds = g_clientfd + 1;
    struct timeval timeout;

    while(1) {
        FD_ZERO(&readfds);
        FD_SET(g_clientfd, &readfds);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        int ret = select(maxfds, &readfds, NULL, NULL, &timeout);
        if (ret ==0 ) {
            /* time out */
            continue;
        }else if (ret == -1) {
            perror("select");
            continue;
        }

        /*  若是数据准备到位，则被至位 */
        if (FD_ISSET(g_clientfd, &readfds)) {
            addrlen = sizeof (peer);
            memset(&peer, 0, addrlen);
            memset(buf, 0, PEER_RECV_BUF);
            int rd_size = recvfrom(g_clientfd, buf, PEER_RECV_BUF, 0, (struct sockaddr*) &peer, &addrlen);

            if (rd_size == -1){
                perror("recvform");
                continue;
            }else if (rd_size == 0){
                log_info("EOF form %s ", ep_tostring(&peer));
                continue;
            }
            log_info("%s", buf);
        }
    }

    return NULL;
}

void* console_loop() 
{
    char *line = NULL;
    size_t len;
    ssize_t read;
    while (fprintf(stdout, ">>> ") && (read = getline(&line, &len, stdin)) != -1) {
        if (read == 1) continue;
        char * cmd = strtok(line, " ");
        if (strncmp(cmd, "peerlist", 4) == 0) {
            // query peer list
            send_query_peerlist_msg();
        }

    }
    return NULL;
}


void send_beating_msg () {
    cJSON* beating_msg = cJSON_CreateObject();
    cJSON_AddNumberToObject(beating_msg, MSG_ID, EMSG_BEATING);
    cJSON_AddStringToObject(beating_msg, CLIENT_ID, "Joe Wei");
    //cJSON_AddNumberToObject(beating_msg, "count", ++i);
    char * msg = cJSON_Print(beating_msg);
    send_udp_msg(g_clientfd, &g_server, msg);
    cJSON_Delete(beating_msg);
}

void send_query_peerlist_msg () {
    cJSON* peerlist_msg = cJSON_CreateObject();
    cJSON_AddNumberToObject(peerlist_msg, MSG_ID, EMSG_PEERLIST);
    char * msg = cJSON_Print(peerlist_msg);
    send_udp_msg(g_clientfd, &g_server, msg);
    cJSON_Delete(peerlist_msg);
}


