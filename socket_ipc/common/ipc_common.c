/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: ipc_common.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
	socket进程间通信数据结构及参数定义
	包括跨设备间TCP进程间通信和本地Unix Domain 
	Socket进程间通信
*Date:	   2019-08-03
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "ipc_common.h"


const char * gc_au8DeviceNoMap[MAX_EXTARN_DEV_NUM] = 
{
	TCP_CLIENT_DEVICE1_IP,
	TCP_CLIENT_DEVICE2_IP,	
};

/**将网络模块的设备号，客户端号，与服务端的客户端地址对应映射**/
const unsigned int gc_as32DeviceModuleMap[TCP_SERVER_LISTEN_MAX_NUM][3] =
{
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT0_POART, DEV1_CLIENT_0_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT1_POART, DEV1_CLIENT_1_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT2_POART, DEV1_CLIENT_2_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT3_POART, DEV1_CLIENT_3_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT4_POART, DEV1_CLIENT_4_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT5_POART, DEV1_CLIENT_5_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT6_POART, DEV1_CLIENT_6_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT7_POART, DEV1_CLIENT_7_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT8_POART, DEV1_CLIENT_8_ADDR,
	TCP_CLIENT_DEVICE1_NO, TCP_DEVICE_CLIENT9_POART, DEV1_CLIENT_9_ADDR,		
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT0_POART, DEV2_CLIENT_0_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT1_POART, DEV2_CLIENT_1_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT2_POART, DEV2_CLIENT_2_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT3_POART, DEV2_CLIENT_3_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT4_POART, DEV2_CLIENT_4_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT5_POART, DEV2_CLIENT_5_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT6_POART, DEV2_CLIENT_6_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT7_POART, DEV2_CLIENT_7_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT8_POART, DEV2_CLIENT_8_ADDR,
	TCP_CLIENT_DEVICE2_NO, TCP_DEVICE_CLIENT9_POART, DEV2_CLIENT_9_ADDR,	
};

/**获取随机数**/
int IPCP_GetRandomReal(int low, int up) 
{ 
   int Result; 
   if (low > up) 
   { 
      int temp = low; 
      low = up; 
      up = temp; 
   } 
     
   Result = rand() % (up-low) + low;

   return Result; 
} 

/**打印消息**/
int IPCP_PrintHexBytes(unsigned char *pu8Bin,int s32Len)
{
	int i;
	if (s32Len<=0)
	{
		return 0;
	}
	
	if (s32Len>2048) 
	{
		s32Len=2048;
	}
	for(i=0;i<s32Len;i++)
	{
		printf("%02x ",pu8Bin[i]);
		if((i+1)%54==0 && i!=0)
		{
			printf("\r\n");
		}
	}
	printf("\r\n");
	return 0;
}


