#ifndef SEND_RR_DATA_H
#define SEND_RR_DATA_H

enum {
	eSERVICE_TP__FORWARD_CLOSE=0x4E,
	eSERVICE_TP__FORWARD_OPEN=0x54,
	eSERVICE_TP__LARGE_FORWARD_OPEN=0x5B,
	eSERVICE_TP__FORWARD_OPEN_RESPONSE = (eSERVICE_TP__FORWARD_OPEN | 0x80),
}eSERVICE_TP;

typedef struct sForwardOpen_t{
	BYTE reserve_1:3;
	BYTE pri:1;
	BYTE tickTime:4;
	USINT timeout_tick;
	
	//O: originator, T: target device
	UDINT o2t_connId;//O->T: T's connection id
	UDINT t2o_connId;//T->O: O's connection id
	
	UINT connSerialNum;
	UINT oVendorId;
	UINT oSerialNum;
	
	USINT connTimeoutMultiplier;
	BYTE reserved[3];
	UDINT o2t_rpi;
	WORD o2tNetConnParams;
	//DWORD o2tLargeNetConnParams;//for LargeForwardOpen

	UDINT t2o_rpi;
	WORD t2oNetConnParams;
	//DWORD t2oLargeNetConnParams;//for LargeForwardOpen
	
	BYTE transportTpOrTrigger;
	USINT connPathSize;
	void *connPath;//real size = connPathSize*2
	
}forwardOpen_t;


#endif