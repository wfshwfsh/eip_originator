#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "eip_common.h"
#include "common.h"
#include "debug.h"

extern U32 g_registerSession_h;



int fill_encap_nop(U8 *header_p, U8 *data_p, void *cmd_data)
{
	encap_header_t *header = (encap_header_t *)header_p;
	
	//1. fill header
	header->command = eENCAP_COMMAND_TP__NOP;
	header->len = 0;
	header->session_handle = 0;//not need & no reply
	header->status = 0;//should be 0
	memset(header->sender_context, 0, 8);//ignore
	header->options = 0;//should be 0
	
	//2. fill data
	//NO DATA or UNUSE DATA
	
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
	if(strlen(SENDER_CONTEXT)<=8)
		strncpy(header->sender_context, SENDER_CONTEXT, strlen(SENDER_CONTEXT));
	else
		strncpy(header->sender_context, SENDER_CONTEXT, 8);
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
	if(strlen(SENDER_CONTEXT)<=8)
		strncpy(header->sender_context, SENDER_CONTEXT, strlen(SENDER_CONTEXT));
	else
		strncpy(header->sender_context, SENDER_CONTEXT, 8);
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
	//NO DATA for eENCAP_COMMAND_TP__UNREGISTER_SESSION
	
	return 0;
}

int fill_encap_sendRRdata(U8 *header_p, U8 *data_p, void *cmd_data)
{
	APPLOG_DDD("fill encapsulation header + cmd_data");
	encap_header_t *header = (encap_header_t *)header_p;
	//1. fill header
	header->command = eENCAP_COMMAND_TP__SEND_RR_DATA;
	header->len = 0;
	//fill session_handle by registerSession Response
	header->session_handle = g_registerSession_h;
	header->status = 0;
	memset(header->sender_context, 0, 8);
	header->options = 0;
	
	//2. fill data
	
	
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




int sendEncapPkt(int sockfd, int cmd_tp, void *cmd_data, int sock_tp)
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
	if(sock_tp == eSOCK_TP__TCP){
		socketSend(sockfd, encap_payload, total_len);
	}
	else{
		struct sockaddr_in bcastAddr;
		socketSetAddr(&bcastAddr, PF_INET, IP_ADDR, PORT);
		udpSocketSend(sockfd, encap_payload, total_len, &bcastAddr, sizeof(bcastAddr));
	}
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





int ParseRecvData(U8 *payload, int data_size)
{
	int ret=0;
	U8 header[LEN_ENCAP_HEADER__24+1]={};
	memset(header, 0, LEN_ENCAP_HEADER__24);
	memcpy(header, payload, LEN_ENCAP_HEADER__24);
	
	//111111111111 parse Encapsulation Header 11111111111111
	//show_payload(header, LEN_ENCAP_HEADER__24, "Header");
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
	
	//222222222222 parse encapsulation data 2222222222222222
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

void sendEncapTest(int sockfd, int cmd_tp, int expect_resp)
{
	char receiveMessage[256] = {};
	int receive_len=0;
	APPLOG_D("------------------------------------");
	memset(receiveMessage, 0, sizeof(receiveMessage));
	sendEncapPkt(sockfd, cmd_tp, NULL, eSOCK_TP__TCP);
	if(expect_resp){
		receive_len=socketRecv(sockfd, receiveMessage, sizeof(receiveMessage));
		show_payload(receiveMessage, receive_len, "RX");
		ParseRecvData(receiveMessage, receive_len);
	}
	else{
		APPLOG_STD_I("no response data in expected");
	}
}

void sendUdpEncapTest(int sockfd, int cmd_tp, int expect_resp)
{
	char receiveMessage[256] = {};
	int receive_len=0;
	APPLOG_D("------------------------------------");
	memset(receiveMessage, 0, sizeof(receiveMessage));
	sendEncapPkt(sockfd, cmd_tp, NULL, eSOCK_TP__UDP);
	if(expect_resp){
		receive_len=socketRecv(sockfd, receiveMessage, sizeof(receiveMessage));
		show_payload(receiveMessage, receive_len, "RX");
		ParseRecvData(receiveMessage, receive_len);
	}
	else{
		APPLOG_STD_I("no response data in expected");
	}
}

void sendEncapTestSendRRdata(int sockfd, void *cmd_data, int expect_resp)
{
	char receiveMessage[256] = {};
	int receive_len=0;
	APPLOG_D("------------------------------------");
	memset(receiveMessage, 0, sizeof(receiveMessage));
	sendEncapPkt(sockfd, eENCAP_COMMAND_TP__SEND_RR_DATA, cmd_data, eSOCK_TP__TCP);
	if(expect_resp){
		receive_len=socketRecv(sockfd, receiveMessage, sizeof(receiveMessage));
		show_payload(receiveMessage, receive_len, "RX");
		ParseRecvData(receiveMessage, receive_len);
	}
	else{
		APPLOG_STD_I("no response data in expected");
	}
}



