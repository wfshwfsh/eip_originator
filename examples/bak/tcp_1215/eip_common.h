#ifndef EIP_COMMON_H
#define EIP_COMMON_H

#define LEN_ENCAP_HEADER__24 24


/* Command description
 * // Encapsulation hear part + Command data part
 * NOP: 0x0000
 *	provide a way for check tcp connection still open
 *  command_data: unuse
 *
 * LIST_IDENTITY: 0x0063
 *	Request:
 *		| Command | len | session_handle | status | SenderContext | options |
 *		| 0x0063  |  0  | ignore         |    0   |       0       |    0    |
 *  Reply:
 *		| Command | len | session_handle | status | SenderContext | options |
 *		| 0x0063  |  0  | ignore         |    0   |       0       |    0    |
 * 		| ItemCount | ListIdentity Items |
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */



enum {
	eENCAP_COMMAND_TP__NOP = 0x0000,
	//0x01-0x03 RA(Reserved for Legacy)
	eENCAP_COMMAND_TP__LIST_SERVICE = 0x0004, //tcp or udp
	//0x05 RA
	//0x06-0x62 Reserve for Future
	eENCAP_COMMAND_TP__LIST_IDENTITY = 0x0063, //tcp or udp
	eENCAP_COMMAND_TP__LIST_INTERFACES = 0x0064, //tcp or udp
	eENCAP_COMMAND_TP__REGISTER_SESSION = 0x0065, //tcp
	eENCAP_COMMAND_TP__UNREGISTER_SESSION = 0x0066,//tcp
	//0x67-0x6e RA
	eENCAP_COMMAND_TP__SEND_RR_DATA = 0x006f,//tcp
	eENCAP_COMMAND_TP__SEND_UNIT_DATA = 0x0070,//tcp
	//0x71 RA
	eENCAP_COMMAND_TP__INDICATE_STATUS = 0x0072,//option (may tcp only)
	eENCAP_COMMAND_TP__CANCEL = 0x0073,//option (may tcp only)
	//0x74-0xc7 RA
	//0xc8-0xffff Reserve for Future
	
}eENCAP_COMMAND_TP;

enum {
	eSUCCESS = 0,
	eENCAP_ERRCODE__INVALID_REQ = 0x0001,
	eENCAP_ERRCODE__UNSUPPORT_REQ = 0x0001,
	eENCAP_ERRCODE__MEM_INSUFFICIENT = 0x0002,
	eENCAP_ERRCODE__INVALID_DATA = 0x0003, //invalid sCommand_data of encapsulation msg
	//0x04-0x63 RA
	eENCAP_ERRCODE__INVALID_SESSION_H = 0x0064, //originator use invalid session handle
	eENCAP_ERRCODE__INVALID_MSG_LEN = 0x0065,
	//0x66-0x68 RA
	eENCAP_ERRCODE__UNSUPPORT_PROTOCOL_VER = 0x0069,
	//0x006a-0xffff Reserve for Future
}eENCAP_ERRCODE;

//struct for encapsolation header
//fixed len 24 byte
typedef struct sEncap_header{
	U16 	command; //use eENCAP_COMMAND_TP
	U16 	len; //fixed len 24
	U32 	session_handle; //get from: REGISTER_SESSION response
	U32 	status;
	char	sender_context[8]; //context len must be 8
	U32		options;
	
}encap_header_t;

//command_data is append behind encapsulation header
typedef struct sCommand_data{
	U32 iface_h;
	U16 timeout;
	void *encap_pkt; //this format is determined by eENCAP_COMMAND_TP
	
}command_data_t;

typedef struct sEncap_RegisterSession_reqData{
	U16 protocol_ver;
	U16 option_flags;
}encap_registerSession_reqData_t;





#endif