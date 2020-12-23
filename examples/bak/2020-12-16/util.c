#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "debug.h"


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


