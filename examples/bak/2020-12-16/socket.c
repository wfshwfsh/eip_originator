#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "debug.h"
#include "common.h"
#include "eip_common.h"

void socketSetAddr(struct sockaddr_in *sockAddr, int addr_family, char *addr, int port)
{
	APPLOG_DDD("addr = %s", addr);
	bzero(sockAddr, sizeof(struct sockaddr_in));
	sockAddr->sin_family = addr_family;
	if(addr == NULL)
		sockAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	else
		sockAddr->sin_addr.s_addr = inet_addr(addr);
    sockAddr->sin_port = htons(port);
}

int socketCreate(int sock_tp)
{
    //socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , sock_tp , 0);

    if (sockfd == -1){
        APPLOG_E("Fail to create a socket.");
    }
	
	return sockfd;
}

int createUdpSocket(void)
{
	int udpSockfd = socketCreate(SOCK_DGRAM);
	return udpSockfd;
}

int createTcpSocket(void)
{
	int tcpSockfd = socketCreate(SOCK_STREAM);
	return tcpSockfd;
}

int socketSetRecvTimeout(int sockfd, int sec, int usec)
{
	struct timeval tv;
	
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	
	if(0!=ret){
		APPLOG_E("failed to setSockOpt (SO_RCVTIMEO)");
	}
	
	return ret;
}

int socketSetBcast(int sockfd)
{
	int broadcast = 1;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast);
	
	if(0!=ret){
		APPLOG_E(" failed to setSockOpt (SO_BROADCAST)");
	}
	
	return ret;
}

//11111111111111111 UDP 1111111111111111111111111111
int socketInitUdpBcast(int sockfd, int addr_family, char *addr, int port)
{
	struct sockaddr_in cliAddr;
	APPLOG_DDD("addr = %s", addr);
	
	/* bind any port */
	socketSetAddr(&cliAddr, addr_family, addr, port);

	int rc = bind(sockfd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
	if(rc<0) {
		APPLOG_E("failed to bind port\n");
	}

	return rc;
}

int udpSocketSend(int sockfd, U8 *send_buf, int send_len, struct sockaddr_in *remoteAddr, int addrSize)
{
	int act_len = sendto(sockfd, send_buf, send_len, 0, 
							(struct sockaddr *) remoteAddr, 
							addrSize);
	
	if(act_len<0) {
		APPLOG_W("failed to send data");
	}
	
	return act_len;
}

int udpSocketRecv(int sockfd/*, U8 *recv_buf*/, struct sockaddr_in *recvAddr)
{
	struct sockaddr_in servAddr;
	int servLen = sizeof(servAddr);
	#define MAX_MSG 100
	char msg[MAX_MSG];
	memset(msg,0,MAX_MSG);
	int act_len = recvfrom(sockfd, msg, MAX_MSG, 0, (struct sockaddr *) &servAddr, &servLen);
	
    if(act_len<0) {
		APPLOG_W("cannot receive data \n");
    }
	else{
		APPLOG_TBD("fill target address info");
		APPLOG_DDD("msg = %s", msg);
	}
	
	return act_len;
}





//22222222222222222 TCP 2222222222222222222222222222
int socketTryConnect(int sockfd, void *addr, int addr_sz)
{
    int err = connect(sockfd,(struct sockaddr *)addr, addr_sz);
    if(err==-1){
        APPLOG_E("Connection error: %s", strerror(errno));
    }
	
	return err;
}

int socketRecv(int sockfd, U8 *recv_buf, int recv_len)
{
	return recv(sockfd, recv_buf, recv_len, 0);
}

int socketSend(int sockfd, U8 *send_buf, int send_len)
{
	return send(sockfd, send_buf, send_len,0);
}



