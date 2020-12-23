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
U32 g_tar_conn_id = 0;
extern U16 g_tar_port=0;

int main(int argc , char *argv[])
{
#if 1
	struct sockaddr_in cliAddr, targetAddr, bcastAddr;

	//int udpSockfd = createUdpSocket();
	//if(socketSetRecvTimeout(udpSockfd, 0, 800000)){
	//	return eEIP_ERR__SOCKETSETOPTION;
	//}
	
	//if(socketSetBcast(udpSockfd)){
	//	return eEIP_ERR__SOCKETSETOPTION;
	//}
	
	//if(socketInitUdp(udpSockfd, AF_INET, INADDR_ANY, 0/*any port*/)){
	//	return eEIP_ERR__SOCKETSETOPTION;
	//}
	
	//bzero(&targetAddr, sizeof(struct sockaddr_in));
	//socketSetAddr(&bcastAddr, PF_INET, IP_ADDR, PORT);
	
	//sendUdpEncapTest(udpSockfd, eENCAP_COMMAND_TP__LIST_IDENTITY, TRUE);

//#else
	int tcpSockfd = createTcpSocket();
	socketSetRecvTimeout(tcpSockfd, 0, 800000);
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socketSetAddr(&addr, PF_INET, IP_ADDR, PORT);

	U8 forwardOpenReq[]={
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0x00, 0x46, 0x00,
		0x54, 0x02, 0x20, 0x06, 0x24, 0x01, 0x05, 0x9b, 0x00, 0x00, 0x00, 0x00, 0x08/*relate to io number*/, 0x40, 0x11, 0x00,
		0x01, 0x00, 0x01, 0x00, 0x99, 0x64, 0xd2, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00,
		0x0a, 0x48, 0x10, 0x27, 0x00, 0x00, 0x06, 0x48, 0x01, 0x0e, 0x34, 0x04, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x20, 0x04, 0x24, 0x7a, 0x2c, 0x66, 0x2c, 0x65, 0x80, 0x04, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	
    //socket的連線
	if(!socketTryConnect(tcpSockfd, &addr, sizeof(addr))){
		
		sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__LIST_SERVICE, NULL, 0, TRUE);
		sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__LIST_IDENTITY, NULL, 0, TRUE);	
		sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__REGISTER_SESSION, NULL, 0, TRUE);
		//sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__UNREGISTER_SESSION, FALSE);
		sendEncapTest(tcpSockfd, eENCAP_COMMAND_TP__SEND_RR_DATA, forwardOpenReq, sizeof(forwardOpenReq), TRUE);
		//g_tar_port=2222;
		if(g_tar_port){
			struct sockaddr_in local_addr;
			int udpSockfd = createUdpSocket();
			if(socketSetRecvTimeout(udpSockfd, 0, 800000)){
				return eEIP_ERR__SOCKETSETOPTION;
			}

			if(socketInitUdp(udpSockfd, AF_INET, INADDR_ANY, g_tar_port)){
				return eEIP_ERR__SOCKETSETOPTION;
			}
			
			while(1)
			{
				sendUdpIoTest(udpSockfd, FALSE);
				usleep(5000);
			}
		}
	}

    close(tcpSockfd);
#endif	
	
    return 0;
}


