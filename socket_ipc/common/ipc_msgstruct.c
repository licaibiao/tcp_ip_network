/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: ipc_msgstruct.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description:命令消息结构体定义和解析
*Date:	   2019-08-03
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "ipc_msgstruct.h"

int IPCPMsg_Debug_Cmd1(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0001_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}

	l_stCmd = (MSG_0X0001_S *)pu8SomeText;
	printf("arrCardNo      = %s \n",l_stCmd->arrCardNo);
	printf("u16Altitude    = 0x%x \n",l_stCmd->u16Altitude);
	printf("u16Direct      = 0x%x \n",l_stCmd->u16Direct);
	printf("u16SensorSpeed = 0x%x \n",l_stCmd->u16SensorSpeed);
	printf("u16SpeedX10    = 0x%x \n",l_stCmd->u16SpeedX10);
	printf("u32Alarm       = 0x%x \n",l_stCmd->u32Alarm);
	printf("u32Latitude    = 0x%x \n",l_stCmd->u32Latitude);
	printf("u32Longtitude  = 0x%x \n",l_stCmd->u32Longtitude);
	printf("u32Status      = 0x%x \n",l_stCmd->u32Status);
	printf("u8CarCor       = 0x%x \n",l_stCmd->u8CarCor);	
	
	return 0;
}
int IPCPMsg_Debug_Cmd2(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0002_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X0002_S*) pu8SomeText;

	printf("u32FileId	   = 0x%x \n",l_stCmd->u32FileId);
	printf("u8DeleteFlag   = 0x%x \n",l_stCmd->u8DeleteFlag);
	printf("u8srcPlat      = 0x%x \n",l_stCmd->u8srcPlat);
	printf("u8StopFlag	   = 0x%x \n",l_stCmd->u8StopFlag);

	return 0;
}
int IPCPMsg_Debug_Cmd3(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0003_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X0003_S*) pu8SomeText;
	
	printf("u8Result = 0x%x \n",l_stCmd->u8Result);
	printf("u8Type   = 0x%x \n",l_stCmd->u8Type);
	
	return 0;
}
int IPCPMsg_Debug_Cmd4(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0004_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X0004_S*) pu8SomeText;

	printf("u32Duration  = 0x%x \n",l_stCmd->u32Duration);
	printf("u8Interval   = 0x%x \n",l_stCmd->u8Interval);

	return 0;
}

int IPCPMsg_Debug_Cmd5(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0005_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X0005_S*) pu8SomeText;
	
	printf("u16CurNum     = 0x%x \n",l_stCmd->u16CurNum);
	printf("u16TotalNum   = 0x%x \n",l_stCmd->u16TotalNum);
	printf("u32MultiId    = 0x%x \n",l_stCmd->u32MultiId);
	printf("u8PlatAddr    = 0x%x \n",l_stCmd->u8PlatAddr);

	return 0;
}

int IPCPMsg_Debug_Cmd6(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0006_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X0006_S*) pu8SomeText;
	printf("u32ModeEvent	  = 0x%x \n",l_stCmd->u32ModeEvent);

	return 0;
}

int IPCPMsg_Debug_Cmd7(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0007_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X0007_S*) pu8SomeText;
	
	printf("u32ParaId     = 0x%x   \n",l_stCmd->u32ParaId);
	printf("u64AlarmFlag  = 0x%llx \n",l_stCmd->u64AlarmFlag);
	printf("u8ParaLen	  = 0x%x   \n",l_stCmd->u8ParaLen);

	return 0;
}

int IPCPMsg_Debug_Cmd8(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0008_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X0008_S*) pu8SomeText;
	
	printf("u32ParaId	  = 0x%x \n",l_stCmd->u32ParaId);
	printf("u32TimeOut    = 0x%x \n",l_stCmd->u32TimeOut);
	printf("u8ParaLen	  = 0x%x \n",l_stCmd->u8ParaLen);

	return 0;
}

int IPCPMsg_Debug_Cmd9(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X0009_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X0009_S*) pu8SomeText;

	printf("u32ParaId	  = 0x%x \n",l_stCmd->u32ParaId);
	printf("u32Payload    = 0x%x \n",l_stCmd->u32Payload);
	printf("u8ParaLen	  = 0x%x \n",l_stCmd->u8ParaLen);

	return 0;
}

int IPCPMsg_Debug_CmdA(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr)
{
	MSG_0X000A_S *l_stCmd = NULL;

	if(NULL==pu8SomeText)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}
	
	l_stCmd = (MSG_0X000A_S*) pu8SomeText;
	printf("u8ParaNum	  = 0x%x \n",l_stCmd->u8ParaNum);

	return 0;
}

