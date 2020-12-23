#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "debug.h"
#include "common.h"
#include "eip_common.h"

#define IP_ADDR "192.168.56.109"
//#define IP_ADDR "192.168.30.199"
#define PORT 44818

U32 g_registerSession_h = 0;

//se: small endian
//be: big endian
U16 bytesToU16_se(U8 *data)
{
    U16 val=0;
	//APPLOG_DDD("data[0] = %02X", data[0]);
	//APPLOG_DDD("data[1] = %02X", data[1]);
	val = (data[0]<<8)|data[1];
	
	//APPLOG_DDD("val = %04X", val);
    return val;
}

U16 bytesToU32_se(U8 *data)
{
    U16 val=0;
	//APPLOG_DDD("data[0] = %02X", data[0]);
	//APPLOG_DDD("data[1] = %02X", data[1]);
	//APPLOG_DDD("data[2] = %02X", data[2]);
	//APPLOG_DDD("data[3] = %02X", data[3]);
	val = (data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3];
	
	//APPLOG_DDD("val = %04X", val);
    return val;
}

U16 bytesToU16_be(U8 *data)
{
    U16 val=0;
	//APPLOG_DDD("data[0] = %02X", data[0]);
	//APPLOG_DDD("data[1] = %02X", data[1]);
	val = (data[1]<<8)|data[0];
	
	//APPLOG_DDD("val = %04X", val);
    return val;
}

U32 bytesToU32_be(U8 *data)
{
    U32 val=0;
	//APPLOG_DDD("data[0] = %02X", data[0]);
	//APPLOG_DDD("data[1] = %02X", data[1]);
	//APPLOG_DDD("data[2] = %02X", data[2]);
	//APPLOG_DDD("data[3] = %02X", data[3]);
	val = (data[3]<<24)|(data[2]<<16)|(data[1]<<8)|data[0];
	
	//APPLOG_DDD("val = %08X", val);
    return val;
}

//bigEndian2smallEndian
U32 bigEndian2smallEndian_U32(U32 val)
{
	U8 tmp[4]={0,0,0,0};
	tmp[0] = (val & 0xff000000)>>24;
	tmp[1] = (val & 0x00ff0000)>>16;
	tmp[2] = (val & 0x0000ff00)>>8;
	tmp[3] = val & 0x000000ff;
	
	return ((tmp[3]<<24)|(tmp[2]<<16)|(tmp[1]<<8)|tmp[0]);
}




int socketCreate(int sock_tp)
{
    //socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , sock_tp , 0);

    if (sockfd == -1){
        APPLOG_STD_E("Fail to create a socket.");
    }
	
	return sockfd;
}

int socketTryConnect(int sockfd, void *addr, int addr_sz)
{
    int err = connect(sockfd,(struct sockaddr *)addr, addr_sz);
    if(err==-1){
        APPLOG_STD_E("Connection error");
    }
	
	return err;
}









void show_payload(U8 *payload, int size, char *tag)
{
	int i;
	printf("\n ======%s payload beg====== \n", tag);
	for(i=0;i<size;i++){
		if((0==i%16) && (0!=i)){
			printf("\n%02X ", payload[i]);
		}
		else{
			printf("%02X ", payload[i]);
		}
	}
	printf("\n ======%s payload end====== \n", tag);
}


int fill_encap_nop(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	return 0;
}

int fill_encap_listService(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	encap_header_t *header = (encap_header_t *)header_p;
	//1. fill header
	header->command = eENCAP_COMMAND_TP__LIST_SERVICE;
	header->len = 0;
	header->session_handle = 0;
	header->status = 0;
	strncpy(header->sender_context, "abcdefgh", 8);
	header->options = 0;
	
	//2. fill data
	//NO DATA for eENCAP_COMMAND_TP__LIST_SERVICE
	
	return 0;
}

int fill_encap_listIdentity(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	encap_header_t *header = (encap_header_t *)header_p;
	//1. fill header
	header->command = eENCAP_COMMAND_TP__LIST_IDENTITY;
	header->len = 0;
	header->session_handle = 0;
	header->status = 0;
	memset(header->sender_context, 0, 8);
	header->options = 0;
	
	//2. fill data
	//NO DATA for eENCAP_COMMAND_TP__LIST_IDENTITY
	
	return 0;
}

int fill_encap_registerSession(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	encap_header_t *header = (encap_header_t *)header_p;
	//1. fill header
	header->command = eENCAP_COMMAND_TP__REGISTER_SESSION;
	header->len = 4;//4 Bytes
	header->session_handle = 0;
	header->status = 0;
	memset(header->sender_context, 0, 8);//???-TBD-???
	header->options = 0;
	
	//2. fill data
	if(NULL==data_p)
	{
		APPLOG_E("Invalid data ???");
		return -1;
	}
	
	encap_registerSession_reqData_t *data = (encap_registerSession_reqData_t *)data_p;
	data->protocol_ver = 1;
	data->option_flags = 0;
	return sizeof(encap_registerSession_reqData_t);
}

int fill_encap_unregisterSession(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	encap_header_t *header = (encap_header_t *)header_p;
	//1. fill header
	header->command = eENCAP_COMMAND_TP__UNREGISTER_SESSION;
	header->len = 0;
	//fill session_handle by registerSession Response
	header->session_handle = g_registerSession_h;
	header->status = 0;
	memset(header->sender_context, 0, 8);
	header->options = 0;
	
	//2. fill data
	//NO DATA for eENCAP_COMMAND_TP__LIST_IDENTITY
	
	return 0;
}

int fill_encap_sendRRdata(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	return 0;
}

int fill_encap_sendUnitData(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	return 0;
}

int fill_encap_indicateStatus(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	return 0;
}

int fill_encap_cancel(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	return 0;
}



int socketSend(int sockfd, int cmd_tp, void *cmd_data)
{
	APPLOG_DDD("cmd_tp = 0x%04X", cmd_tp);
	
	int ret=0;
	int total_len=0;
	int cmd_data_len=0;
	int cmd_header_len = LEN_ENCAP_HEADER__24;
	U8 header[LEN_ENCAP_HEADER__24+1] = {};
	U8 *data=NULL;//fixed reqeust data <=> cmd_data unfixed
	U8 *encap_payload=NULL;
	
	//1. fill cmd_data by each encapsulation cmd type
	//2. update encapsulation packet len
	switch(cmd_tp){
		
		case eENCAP_COMMAND_TP__NOP:
			cmd_data_len = fill_encap_nop(header, data, cmd_data);
			break;
		case eENCAP_COMMAND_TP__LIST_SERVICE:
			cmd_data_len = fill_encap_listService(header, data, cmd_data);
			break;
		case eENCAP_COMMAND_TP__LIST_IDENTITY:
			cmd_data_len = fill_encap_listIdentity(header, data, cmd_data);
			break;
		case eENCAP_COMMAND_TP__REGISTER_SESSION:
			//protocol_ver(2B) | option_flag(2B)
			data = malloc(sizeof(encap_registerSession_reqData_t));
			cmd_data_len = fill_encap_registerSession(header, data, cmd_data);
			break;
		case eENCAP_COMMAND_TP__UNREGISTER_SESSION:
			cmd_data_len = fill_encap_unregisterSession(header, data, cmd_data);
			break;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case eENCAP_COMMAND_TP__SEND_RR_DATA:
			//iface_h | timeout | encap_pkts(several types)
			cmd_data_len = fill_encap_sendRRdata(header, data, cmd_data);
			break;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case eENCAP_COMMAND_TP__SEND_UNIT_DATA:
			cmd_data_len = fill_encap_sendUnitData(header, data, cmd_data);
			break;
		case eENCAP_COMMAND_TP__INDICATE_STATUS://???
			cmd_data_len = fill_encap_indicateStatus(header, data, cmd_data);
			break;
		case eENCAP_COMMAND_TP__CANCEL://???
			cmd_data_len = fill_encap_cancel(header, data, cmd_data);
			break;
		default:
			APPLOG_W("unknown command type ???");
			break;
	}
	
	//3. check encapsulation data in expect
	if(cmd_data_len < 0){
		APPLOG_STD_E("invalid encapsulation data len");
		ret = -1;
		goto RET;
	}
	else{
		encap_payload = malloc(sizeof(U8)*(LEN_ENCAP_HEADER__24+cmd_data_len));
		if(NULL == encap_payload){
			APPLOG_STD_E("invalid encapsulation data len");
			ret = -1;
			goto RET;
		}
		
		memcpy(encap_payload, header, LEN_ENCAP_HEADER__24);
		if(cmd_data_len>0){
			memcpy(encap_payload+LEN_ENCAP_HEADER__24, data, cmd_data_len);
		}
	}
	total_len = cmd_header_len + cmd_data_len;
	APPLOG_D("total_len = %d", total_len);
	
	//4. socket send
	send(sockfd, encap_payload, total_len,0);
	
	show_payload(encap_payload, total_len, "TX");
	
RET:
	if(NULL != data){
		data=NULL;
		free(data);
	}
	
	if(NULL != encap_payload){
		encap_payload=NULL;
		free(encap_payload);
	}
	
	return ret;
}

int socketRecv(int sockfd, U8 *recv_buf, int recv_len)
{
	//recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
	return recv(sockfd, recv_buf, recv_len, 0);
}




int ParseRecvData(U8 *payload, int data_size)
{
	int ret=0;
	U8 header[LEN_ENCAP_HEADER__24+1]={};
	memset(header, 0, LEN_ENCAP_HEADER__24);
	memcpy(header, payload, LEN_ENCAP_HEADER__24);
	//show_payload(header, LEN_ENCAP_HEADER__24, "Header");
	//parse Encapsulation Header
	U16 cmd_tp = bytesToU16_be((U8 *)&header[0]);
	U16 len = bytesToU16_be((U8 *)&header[2]);
	U32 registerSession_h = bytesToU32_be((U8 *)&header[4]);
	U32 status = bytesToU32_se((U8 *)&header[8]);
	U8 sender_context[8] = {};
	memcpy(sender_context, (void*)&header[12], 8);
	U32 options = bytesToU32_se((U8 *)&header[20]);
	
	//registerSession_h is valid, only if status=0 in registerSession response
	if(!status && (eENCAP_COMMAND_TP__REGISTER_SESSION==cmd_tp))
	{
		APPLOG_STD_I("registerSession_h = %08X", registerSession_h);
		g_registerSession_h = registerSession_h;
	}
	
	
	if(LEN_ENCAP_HEADER__24 == data_size){
		return 0;
	}
	
	APPLOG_TBD("parse encapsulation data");
	switch(cmd_tp){
		case eENCAP_COMMAND_TP__NOP:
			APPLOG_TBD("eENCAP_COMMAND_TP__NOP");
			break;
		case eENCAP_COMMAND_TP__LIST_SERVICE:
			APPLOG_TBD("eENCAP_COMMAND_TP__LIST_SERVICE");
			break;
		case eENCAP_COMMAND_TP__LIST_IDENTITY:
			APPLOG_TBD("eENCAP_COMMAND_TP__LIST_IDENTITY");
			break;
		case eENCAP_COMMAND_TP__REGISTER_SESSION:
			APPLOG_TBD("eENCAP_COMMAND_TP__REGISTER_SESSION");
			break;
		case eENCAP_COMMAND_TP__UNREGISTER_SESSION:
			APPLOG_TBD("eENCAP_COMMAND_TP__UNREGISTER_SESSION");
			break;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case eENCAP_COMMAND_TP__SEND_RR_DATA:
			APPLOG_TBD("eENCAP_COMMAND_TP__SEND_RR_DATA");
			break;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case eENCAP_COMMAND_TP__SEND_UNIT_DATA:
			APPLOG_TBD("eENCAP_COMMAND_TP__SEND_UNIT_DATA");
			break;
		case eENCAP_COMMAND_TP__INDICATE_STATUS://???
			APPLOG_TBD("eENCAP_COMMAND_TP__INDICATE_STATUS");
			break;
		case eENCAP_COMMAND_TP__CANCEL://???
			APPLOG_TBD("eENCAP_COMMAND_TP__CANCEL");
			break;
		default:
			APPLOG_W("unknown command type ???");
			break;
	}
	
RET:
	
	return ret;
}

void socketTest(int sockfd, int cmd_tp)
{
	char receiveMessage[256] = {};
	int receive_len=0;
	APPLOG_D("------------------------------------");
	memset(receiveMessage, 0, sizeof(receiveMessage));
	socketSend(sockfd, cmd_tp, NULL);
	receive_len=socketRecv(sockfd, receiveMessage, sizeof(receiveMessage));
	show_payload(receiveMessage, receive_len, "RX");
	ParseRecvData(receiveMessage, receive_len);
}





int main(int argc , char *argv[])
{
	struct sockaddr_in addr;
	int sockfd = socketCreate(SOCK_STREAM);	
	struct timeval tv;
	
	tv.tv_sec = 0;
	tv.tv_usec = 800000;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	
	bzero(&addr,sizeof(addr));
	addr.sin_family = PF_INET;
    addr.sin_addr.s_addr = inet_addr(IP_ADDR);
    addr.sin_port = htons(PORT);
	
    //socket的連線
	if(!socketTryConnect(sockfd, &addr, sizeof(addr)))
	{
		socketTest(sockfd, eENCAP_COMMAND_TP__LIST_SERVICE);
		socketTest(sockfd, eENCAP_COMMAND_TP__LIST_IDENTITY);	
		socketTest(sockfd, eENCAP_COMMAND_TP__REGISTER_SESSION);
		socketTest(sockfd, eENCAP_COMMAND_TP__UNREGISTER_SESSION);

		//APPLOG_D("%s",receiveMessage);
		//APPLOG_D("close Socket\n");
	}
	
    close(sockfd);
    return 0;
}


