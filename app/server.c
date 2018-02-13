#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include "_rb_tree.h"
#include "_logging.h"
#include "_endpoint.h"
#include "cJSON.h"
#include "Const_str.h"
#include "_msg.h"
#include "_msghandler.h"
#include "_peer_manager.h"

#define RECV_BUFFSIZE 1024

static RBTree g_msg_handler_mapping;
static int g_socketfd;
static int process_udp (Endpoint* endpoint, const char * buf) 
{
	log_info(buf);
	cJSON* root = cJSON_Parse(buf);
	if (!root) return -1;
	cJSON* msg_id = cJSON_GetObjectItem(root, MSG_ID);
	int msg = msg_id->valueint;

	Handler handler = find_handler(&g_msg_handler_mapping, msg);
	if (handler) {
		handler(endpoint, root);
	}
	cJSON_Delete(root);
}

static void udp_receive_loop(int listen_socket) {
	Endpoint peer;
	socklen_t addrlen;
	int rd_size;
	char buf[RECV_BUFFSIZE];

	for(;;) {

		addrlen = sizeof(peer);
		memset(&peer, 0, addrlen);
		memset(buf, 0, RECV_BUFFSIZE);
		// recvfrom 的 第四个参数是填入from端的socket信息。
		// 填了多少由 第五个参数指出。同时第五个参数也指明能填多少个字节，是一个 值-结果参数。
		rd_size = recvfrom(listen_socket, buf, RECV_BUFFSIZE, 0, (struct sockaddr*)&peer, &addrlen);
		if (rd_size == -1){
			perror("recvfrom faild");
			break;
		}else if (rd_size == 0) {
			continue;
		}
		process_udp(&peer, buf);
	}
}
static int on_beating (Endpoint* from, void * params) 
{	
	cJSON* root = (cJSON*)params;
	cJSON* client = cJSON_GetObjectItem(root, CLIENT_ID);
	
	update_peer(client->valuestring, from);
	// insert or update the client

}

static int peer_list (Endpoint* from, void * params) 
{
	// 获取所有的 peer
	int peers_size = peer_size();
	int i;
	Peer *peer[peers_size];
	get_all_peer(peer);

	cJSON * array = cJSON_CreateArray();
	for (i=0; i<peers_size; ++i) {
		cJSON * jpeer = cJSON_CreateObject();
		cJSON_AddStringToObject(jpeer, CLIENT_ID, peer[i]->id);
		cJSON_AddStringToObject(jpeer, PEER_IO, ep_tostring(&peer[i]->io));
		cJSON_AddItemToArray(array, jpeer);
	}
	
	send_udp_msg(g_socketfd, from, cJSON_Print(array));

	//free(peer);
	cJSON_Delete(array);

	// 
}

int main(int argc, char** argv) 
{
	int port = atoi(argv[1]);
	int ret;
	const char * host = "0.0.0.0";
	Endpoint server;

	/* regist msg handler */
	init_mhmap(&g_msg_handler_mapping);
	add_handler(&g_msg_handler_mapping, EMSG_BEATING, &on_beating);
	add_handler(&g_msg_handler_mapping, EMSG_PEERLIST, &peer_list);
	log_info("add_handler done");
	/* regist msg handler */

	/* init peer list */
	init_peerlist();
	log_info("init_peerlist\n");
	/* init the socktet */
	ep_frompair(&server, host, port);
	g_socketfd= socket(AF_INET, SOCK_DGRAM,0);
	if (g_socketfd == -1 ) {
		perror("sockt");
		exit(EXIT_FAILURE);
	}

	ret = bind(g_socketfd, (const struct sockaddr* ) &server, sizeof(server));
	if (ret == -1){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	printf("server listening on port %d ...\n", port);

	// 接受信息。
	udp_receive_loop(g_socketfd);
}