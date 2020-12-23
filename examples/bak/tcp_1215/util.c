#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

U16 convertToU16(U8 *data)
{
    U16 val=0;
	APPLOG_DDD("data[0] = %02X", data[0]);
	APPLOG_DDD("data[1] = %02X", data[1]);
	val = (data[0]<<8)|data[1];
	
	APPLOG_DDD("val = %04X", val);
    return val;
}


