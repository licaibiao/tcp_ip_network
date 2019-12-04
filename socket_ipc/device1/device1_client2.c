/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: tcp_client1.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
*Date:	   2019-06-22
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "ipc_interface.h"
#include "ipc_msgstruct.h"
#include "ipc_common.h"

#define CLIENT_MODULE_ADDR		   DEV1_CLIENT_2_ADDR
#define DELAY_FOR_DEBUG_US        (1000*1000)

/**消息处理**/
MSG_HANDLE_HOOK_S g_astLocalMsgTable[] =
{
	{QUEUE_DEBUG_CMD1, IPCPMsg_Debug_Cmd1},
	{QUEUE_DEBUG_CMD2, IPCPMsg_Debug_Cmd2},
	{QUEUE_DEBUG_CMD3, IPCPMsg_Debug_Cmd3},
	{QUEUE_DEBUG_CMD4, IPCPMsg_Debug_Cmd4},
	{QUEUE_DEBUG_CMD5, IPCPMsg_Debug_Cmd5},
	{QUEUE_DEBUG_CMD6, IPCPMsg_Debug_Cmd6},
	{QUEUE_DEBUG_CMD7, IPCPMsg_Debug_Cmd7},
	{QUEUE_DEBUG_CMD8, IPCPMsg_Debug_Cmd8},
	{QUEUE_DEBUG_CMD9, IPCPMsg_Debug_Cmd9},
	{QUEUE_DEBUG_CMDA, IPCPMsg_Debug_CmdA},
    {0,NULL},
};


/**********************************
linux ctrl + C 会产生 SIGINT信号
接收到SIGINT 信号进入该函数
**********************************/
void stop(int signo)
{
	int i = 0;
	printf(" stop \n");

	//g_s32RWStates = IPCP_FALSE;
	//if(g_s32SocketFd>0)
	//{
	//	g_s32SocketFd = 0;
	//}
	
	_exit(0);
}

/*********************************************
当客户端断开连接的时候，
在服务端socket send进程可以收到收到信号SIGPIPE，
收到SIGPIPE信号进入该函数结束创建的线程。
**********************************************/
void signal_pipe(int signo)
{
	
}

/******************************************************** 
Function:	 ClientSendMsg  
Description: 客户端(某一进程)发送数据包
Input:	
	s32TargModuleAddr ： 需要发送到的模块地址
OutPut: none
Return: 0 成功;非0 异常
Others:	
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
int ClientSendMsg(int s32TargModuleAddr)
{
	int l_s32Res = 0;
	MSG_PACK_S l_stMsg = {0};
	MSG_0X0001_S l_stMsgSend = {0};

	if((s32TargModuleAddr<LOCAL_CLIENT_1_ADDR)&&(s32TargModuleAddr>LOCAL_CLIENT_MAX_ADDR))
	{
		printf("%s %d input para error %d \n",__FUNCTION__,__LINE__,s32TargModuleAddr);
		return -1;
	}


	l_stMsgSend.arrCardNo;
	l_stMsgSend.u16Altitude   = 0x22;
	l_stMsgSend.u16Direct     = 0x22;
	l_stMsgSend.u16SensorSpeed= 0x22;
	l_stMsgSend.u16SpeedX10   = 0x11;
	l_stMsgSend.u32Alarm      = 0x55;
	l_stMsgSend.u32Latitude   = 0x66;
	l_stMsgSend.u32Longtitude = 0x77;
	l_stMsgSend.u32Status     = 0x88;
	l_stMsgSend.u8CarCor	  = 0x99;

	l_stMsg.CmdId    = QUEUE_DEBUG_CMD1;
	l_stMsg.Data     = (unsigned char*)&l_stMsgSend;
	l_stMsg.Len      = sizeof(MSG_0X0001_S);
	l_stMsg.SrcAddr  = CLIENT_MODULE_ADDR;
	l_stMsg.TargAddr = s32TargModuleAddr;
	IPCP_Arch_Msg_PackSend(CLIENT_MODULE_ADDR,&l_stMsg);

	return 0;

}

/******************************************************** 
Function:	 ClientRecvMsg  
Description: 客户端(某一进程)接收数据包，并对数据包进行解析
Input:	
OutPut: none
Return: 0 成功;非0 异常
Others:	
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
int ClientRecvMsg(void)
{
	int i = 0;
	int l_s32RecvLen = 0;
	int l_s32SrcAddr = 0;
	int l_s32CmdId   = 0;
	int l_s32TextLen = 0;
	int l_s32FlowNum = 0;
	int l_u32IPCPMsgNum = 0;
	
	unsigned char *l_pu8MsgBody = NULL;
	ARCH_MSG_S l_stMsg = {0};

	l_s32RecvLen = IPCP_Arch_Msg_Recv(CLIENT_MODULE_ADDR,&l_stMsg);

	if(l_s32RecvLen>0)
	{		
		IPCP_Arch_Msg_PlatformStartP(&l_pu8MsgBody, l_stMsg.SomeText);

		l_u32IPCPMsgNum = ARRAY_SIZE(g_astLocalMsgTable);
		l_s32SrcAddr = IPCP_Arch_Msg_AnalyzeGetSrcAddr(&l_stMsg);
		l_s32CmdId   = IPCP_Arch_Msg_AnalyzeGetCmdID(&l_stMsg);
		l_s32TextLen = IPCP_Arch_Msg_AnalyzeGetLen(&l_stMsg);
		l_s32FlowNum = IPCP_Arch_Msg_AnalyzeGetRecFlow(&l_stMsg);

		printf("l_s32SrcAddr = %d \n",l_s32SrcAddr);
		printf("l_s32CmdId   = %d \n",l_s32CmdId);
		printf("l_s32TextLen = %d \n",l_s32TextLen);
		printf("l_s32FlowNum = %d \n",l_s32FlowNum);

		for(i = 0; i < l_u32IPCPMsgNum; i++)
		{
			if(l_s32CmdId == g_astLocalMsgTable[i].u32MsgID)
			{
				if(NULL!=g_astLocalMsgTable[i].pFuncHandle)
				{
					g_astLocalMsgTable[i].pFuncHandle(l_pu8MsgBody, l_s32TextLen,l_s32SrcAddr);
				}
				break;
			}
		}
	}

	return 0;
}



int main(int argc,char *argv[])
{
	int l_s32Delay    = 0;
	int l_s32Addr     = 0;
	int l_as32SendAddr[3] ={0};

	l_as32SendAddr[0] = LOCAL_CLIENT_2_ADDR;
	l_as32SendAddr[1] = DEV1_CLIENT_1_ADDR,
	l_as32SendAddr[2] = LOCAL_CLIENT_1_ADDR;
	
	/**注册 SIGPIPE信号**/
	signal(SIGPIPE,signal_pipe); 

	/**注册SIGINT 信号**/
	signal(SIGINT,stop); 

	while(1)
	{
	
		l_s32Addr = IPCP_GetRandomReal(0,3);
		if((l_s32Addr>=0)&&(l_s32Addr<=2))
		{
			ClientSendMsg(l_as32SendAddr[l_s32Addr]);
		}
		
		ClientRecvMsg();
		
		l_s32Delay = IPCP_GetRandomReal(50,100);
		usleep(l_s32Delay*1000);
	}


	return 0;
}

