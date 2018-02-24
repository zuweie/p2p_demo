#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "_endpoint.h"
#define INET_PORTSTRLEN 5
#define STRBUF_SIZE (INET_ADDRSTRLEN + INET_PORTSTRLEN + 1)

char stringBuf[STRBUF_SIZE];

extern const char* ep_tostring(Endpoint* ep) 
{
    snprintf(stringBuf, STRBUF_SIZE, "%s:%d", inet_ntoa(ep->sin_addr), ntohs(ep->sin_port));
    return stringBuf;
}

extern int ep_frompair(Endpoint* ep, const char* host, short port) 
{
    memset(ep, 0, sizeof(Endpoint));
    ep->sin_family = AF_INET;
    ep->sin_addr.s_addr = inet_addr(host);
    ep->sin_port = htons(port);
    return 0;
}
extern int ep_fromstring(Endpoint* ep, const char* hostport)
{
    char string[STRBUF_SIZE];
    char *host = NULL;
    char *port = NULL;
    sprintf(string, "%s", hostport);
    // str：在第一次被调用的时间str是传入需要被切割字符串的首地址；在后面调用的时间传入NULL。 
    // 这么嗨神奇，肯定是存在了一个全局的变量了。
    host = strtok(hostport, ":");
    port = strtok(NULL, ":");
    if (host == NULL || port == NULL){
        host = "255.255.255.255";
        port = "0";
    }
    return ep_frompair(ep, host, atoi(port));
}
