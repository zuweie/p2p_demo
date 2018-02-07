#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include "_logging.h"
#include "_endpoint.h"

#define RECV_BUFFSIZE 1024

void udp_receive_loop(int listen_socket) {
	Endpoint peer;
	socklen_t addrlen;
	char buf[RECV_BUFFSIZE];
	int rd_size;
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
		log_info("RECV msg : %s from %s ", buf, ep_tostring(&peer));
	}
}

int main(int argc, char** argv) 
{
	int port = atoi(argv[1]);
	int ret;
	const char * host = "0.0.0.0";
	Endpoint server;

	ep_frompair(&server, host, port);
	int socketfd = socket(AF_INET, SOCK_DGRAM,0);
	if (socketfd == -1 ) {
		perror("sockt");
		exit(EXIT_FAILURE);
	}

	ret = bind(socketfd, (const struct sockaddr* ) &server, sizeof(server));
	if (ret == -1){
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// 接受信息。
	udp_receive_loop(socketfd);
}