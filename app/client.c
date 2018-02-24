#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>
#include "_endpoint.h"
#include "_logging.h"
#include "cJSON.h"
#include "_msg.h"
#include "_msghandler.h"
#include "Const_str.h"
#include "_peer_manager.h"

#define MSGBUF_SIZE 129
#define PEER_RECV_BUF 1024

static int g_clientfd;
static Endpoint g_server;
static RBTree g_msg_handler_mapping;
static char _id[128];

// 此函数不断给服务器发送心跳包
void* keepalive_loop();
void* receive_loop();
void* console_loop();
void send_beating_msg();
void send_query_peerlist_msg();
void process_udp(Endpoint* ep, const char * buf);
void chat_to_peer (Endpoint* to, const char* msg);

static int on_peerlist (Endpoint* ep, void * params) 
{
    cJSON* root = (cJSON*) params;
    cJSON* peer_array = cJSON_GetObjectItem(root, PEER_LIST);
    int size = cJSON_GetArraySize(peer_array);
    int i;
    for (i=0; i<size; ++i ) {
        cJSON* item = cJSON_GetArrayItem(peer_array, i);
        cJSON* peer_id = cJSON_GetObjectItem(item, PEER_ID);
        cJSON* peer_io = cJSON_GetObjectItem(item, PEER_IO);
        //log_info("peer : %s / %s \n", peer_id->valuestring, peer_io->valuestring);
        Endpoint peer;
        ep_fromstring(&peer, peer_io->valuestring);
        update_peer(peer_id->valuestring, &peer);
    }
    print_all_peer();
}

static int on_chat(Endpoint* ep, void * params )
{
    cJSON* root = (cJSON*) params;
    cJSON* chat = cJSON_GetObjectItem(root, CHAT_CONTENT);
    char * msg = chat->valuestring;
    log_info("you get a msg (from %s) : %s \n", ep_tostring(ep), msg);

}



int main (int argc, char** argv) 
{
    
    log_setlevel(INFO);

    if (argc != 3) {
        fprintf(stderr, "Usage: %s peer_id server:port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(_id, 0, 128);

    strncpy(_id, argv[1], 128);

    ep_fromstring(&g_server, argv[2]);

    log_info("_id : %s, host : %s ", _id, ep_tostring(&g_server));

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
    add_handler(&g_msg_handler_mapping, EMSG_ONCHAT, &on_chat);

    /* 3 init peer manager*/
    init_peerlist();

     
    pthread_t keepalive_pid, console_pid, receive_pid;
    int ret;
    /*
    int ret = pthread_create(&keepalive_pid, NULL, &keepalive_loop, NULL);
    if (ret != 0) {
        perror("keepalive");
        goto clean;
    }
    */
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
    //pthread_join(keepalive_pid, NULL);
    pthread_join(console_pid,   NULL);
    pthread_join(receive_pid,   NULL);
clean:
    return EXIT_FAILURE;
}

void* keepalive_loop() {
    /*
    for(;;){
        sleep(6);
        //log_info("send a message to server\n");
        send_beating_msg();
    }
    */
    send_beating_msg();

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

    for(;;) {
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
            //log_info("%s", buf);
            process_udp (&peer, buf);
        }
    }

    return NULL;
}

void* console_loop() 
{
    char line[1023];
    size_t len = 1024;
    ssize_t read;
    while (fprintf(stdout, ">>> ") && (read = fgets(&line, &len, stdin)) != -1) {
        if (read == 1) continue;
        char * cmd = strtok(line, " ");
        
        if (strncmp(cmd, "peerlist", 8) == 0) {
            // query peer list
            send_query_peerlist_msg();
        }else if (strncmp(cmd, "chatto", 6) == 0) {
            char * peer_id = strtok(NULL, " ");
            Peer* peer = find_peer(peer_id);
            if (peer) {
                char *msg = strtok(NULL, " ");
                // TODO : send to msg to peer;
                chat_to_peer(&(peer->io), msg);
            }else{
                log_info(" Can`t find %s in your contacts!\n may be you should be update you contracts by type peerlist\n", peer_id);
            }
        }else if (strncmp(cmd, "keeplive", 8) ==0 ){
            keepalive_loop();
        }

    }
    return NULL;
}


void send_beating_msg () {
    cJSON* beating_msg = cJSON_CreateObject();
    cJSON_AddNumberToObject(beating_msg, MSG_ID, EMSG_BEATING);
    cJSON_AddStringToObject(beating_msg, PEER_ID, _id);
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

void chat_to_peer (Endpoint* to, const char* content) {
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, MSG_ID, EMSG_ONCHAT);
    cJSON_AddStringToObject(root, CHAT_CONTENT, content);
    char * msg = cJSON_Print(root);
    send_udp_msg(g_clientfd, to, msg);
    log_info("to_ep : %s\nto_msg:%s\n", ep_tostring(to), msg);
    cJSON_Delete(root);
}
void process_udp(Endpoint* ep, const char * buf) {
    log_info("udp:%s\n", buf);
    cJSON* root = cJSON_Parse(buf);
    cJSON* msg  = cJSON_GetObjectItem(root, MSG_ID);
    int msg_id = msg->valueint;
    Handler handler = find_handler(&g_msg_handler_mapping, msg_id);
    handler && handler(ep, root);
    cJSON_Delete(root);
}


