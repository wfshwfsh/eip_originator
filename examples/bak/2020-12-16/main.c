#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include "debug.h"
#include "common.h"
#include "eip_common.h"



U32 g_registerSession_h = 0;

int main(int argc , char *argv[])
{
#if 1
	struct sockaddr_in cliAddr, targetAddr, bcastAddr;

	int udpSockfd = createUdpSocket();
	if(socketSetRecvTimeout(udpSockfd, 0, 800000)){
		return eEIP_ERR__SOCKETSETOPTION;
	}	
	
	if(socketSetBcast(udpSockfd)){
		return eEIP_ERR__SOCKETSETOPTION;
	}
	
	if(socketInitUdpBcast(udpSockfd, AF_INET, INADDR_ANY, 0/*any port*/)){
		return eEIP_ERR__SOCKETSETOPTION;
	}
	
	bzero(&targetAddr, sizeof(struct sockaddr_in));
	socketSetAddr(&bcastAddr, PF_INET, IP_ADDR, PORT);
	
	sendUdpEncapTest(udpSockfd, eENCAP_COMMAND_TP__LIST_IDENTITY, TRUE);

#else
	int tcpSockfd = createTcpSocket();
	socketSetRecvTimeout(tcpSockfd, 0, 800000);
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socketSetAddr(&addr, PF_INET, IP_ADDR, PORT);

	U8 cmd_data[60];
    //socket的連線
	if(!socketTryConnect(tcpSockfd, &addr, sizeof(addr))){
		
		sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__LIST_SERVICE, TRUE);
		sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__LIST_IDENTITY, TRUE);	
		sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__REGISTER_SESSION, TRUE);
		sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__UNREGISTER_SESSION, FALSE);
		sendEncapTestSendRRdata(tcpSockfd, cmd_data, TRUE);
	}

    close(tcpSockfd);
#endif	
	
    return 0;
}


