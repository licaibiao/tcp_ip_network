/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: ipc_interface.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description:客户端网络连接，数据收发，消息解析函数接口定义实现
*Date:	   2019-08-03
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ipc_interface.h"

static unsigned char gs_as8RecvData[MSG_TEXT_SIZE] = {0};
static unsigned short gs_u16FlowNum =0;
static int gs_s32SocketId = -1;

typedef enum
{
	UNIX_DOMAIN = 1,
	TCP_SOCKET  = 2,
	UDP_SOCKET  = 3,
}SOCKET_TYPE_E;

/**申明内部函数**/
static int IPCP_Internal_NeedMoreData(unsigned char *pu8Addr,int s32Len,int *ps32NeedLen);
static int IPCP_Internal_TcpCheckClientPort(int s32ModuleAddr);
static int IPCP_Internal_TcpClientConnetToServer(int s32ModuleAddr);
static int IPCP_Internal_UnixDomainConnetToServer(int s32ModuleAddr);
static int IPCP_Internal_CheckNetType(int s32ModuleAddr);
static int IPCP_Internal_InitSocket(int s32ModuleAddr);
static int IPCP_Internal_Msg_Send(ARCH_MSG_S *pstMsg,int s32Len);
static int IPCP_Internal_RecieveData(int s32ModuleAddr, int *ps32Datalen);


/**获取流水号**/
int IPCP_Arch_Msg_AnalyzeGetFlowNum()
{
    return gs_u16FlowNum;             //已经是加1的值
}

/**获取消息内容开始位置**/
void IPCP_Arch_Msg_PlatformStartP(unsigned char ** p, unsigned char* Data)
{
	*p= &Data[MSG_CONTENT_OFFSET];
}

/**获取接收到数据中的流水号**/
int IPCP_Arch_Msg_AnalyzeGetRecFlow(ARCH_MSG_S *pstMsg)
{
    unsigned char *l_pu8DestText=pstMsg->SomeText;
    if(NULL==pstMsg)
    {
		return IPCP_FALSE;
	}
    
    return (l_pu8DestText[MSG_SERIAL_NUM_ADDR]<<8|l_pu8DestText[MSG_SERIAL_NUM_ADDR+1]);
}

/**改变源地址**/
int IPCP_Arch_Msg_ChangeSrcAddr(ARCH_MSG_S *pstMsg,unsigned  char SrcAddr)
{
    unsigned char *l_pu8DestText=pstMsg->SomeText;

    if(NULL==pstMsg||0==SrcAddr)
    {
		return IPCP_FALSE;
	}
    
    l_pu8DestText[MSG_SRC_ADDR]=SrcAddr;
	
    return IPCP_TRUE;
}

/**获取目标地址**/
int IPCP_Arch_Msg_ChangeTargAddr(ARCH_MSG_S *pstMsg,unsigned  char TargAddr)
{
    unsigned char *l_pu8DestText=pstMsg->SomeText;

    if(NULL==pstMsg||0==TargAddr)
    {
		 return IPCP_FALSE;
	}
       
    l_pu8DestText[MSG_TAR_ADDR]=TargAddr;
	
    return IPCP_TRUE;
}

/**获取源地址**/
int IPCP_Arch_Msg_AnalyzeGetSrcAddr(ARCH_MSG_S *pstMsg)
{
    unsigned char *l_pu8DestText=pstMsg->SomeText;

    if(NULL==pstMsg)
    {
        return IPCP_FALSE;
	}

    return l_pu8DestText[MSG_SRC_ADDR];
}

/**获取目标地址**/
int IPCP_Arch_Msg_AnalyzeGetTargAddr(ARCH_MSG_S *pstMsg)
{
    unsigned char *l_pu8DestText=pstMsg->SomeText;

    if(NULL==pstMsg)
    {
		return IPCP_FALSE;
	}
    
    return l_pu8DestText[MSG_TAR_ADDR];
}


/**获取消息ID**/
int IPCP_Arch_Msg_AnalyzeGetCmdID(ARCH_MSG_S *pstMsg)
{
    unsigned char *l_pu8DestText=pstMsg->SomeText;

    if(NULL==pstMsg)
    {
		return IPCP_FALSE;
	}
        
    return l_pu8DestText[MSG_CMID_ADDR]<<8|l_pu8DestText[MSG_CMID_ADDR+1]<<0;
}


/**获取内容长度**/
int IPCP_Arch_Msg_AnalyzeGetLen(ARCH_MSG_S *pstMsg)
{
    unsigned char *l_pu8DestText=pstMsg->SomeText;

    if(NULL==pstMsg)
    {
		return IPCP_FALSE;
	}
    
    return (l_pu8DestText[MSG_MSG_LEN_ADDR]<<8|l_pu8DestText[MSG_MSG_LEN_ADDR+1]);
}

/**发送一条命令**/
int IPCP_Arch_Msg_PackSend(int s32ModuleAddr, MSG_PACK_S *pstMsg)
{
	int l_s32Ret = 0;
    ARCH_MSG_S l_stMsg={0};
    unsigned  char *pu8Data = NULL;
    unsigned short l_u16DataLen=0;
	unsigned short l_s16FlowNum = 0;

	if(gs_s32SocketId <=0)
	{
		l_s32Ret = IPCP_Internal_InitSocket(s32ModuleAddr);
		if(0!=l_s32Ret)
		{
			return -1;
		}
	}

	pu8Data 	 = l_stMsg.SomeText;
	l_s16FlowNum = gs_u16FlowNum++;

    if(0==pstMsg->SrcAddr||0==pstMsg->TargAddr)
    {
		return IPCP_FALSE;
	}

	/**消息头**/
    *(pu8Data+0)= (unsigned  char)(QUEUE_MSG_HEAD >>8);              
    *(pu8Data+1)= (unsigned  char)(QUEUE_MSG_HEAD >>0);
    *(pu8Data+2)= pstMsg->SrcAddr;
    *(pu8Data+3)= pstMsg->TargAddr;
	
	/**消息流水号**/
    *(pu8Data+4)= (l_s16FlowNum%65535)>>8;
    *(pu8Data+5)= (l_s16FlowNum%65535)>>0;                     
    l_u16DataLen+=6;
    *(pu8Data+l_u16DataLen+0)= pstMsg->CmdId>>8;
    *(pu8Data+l_u16DataLen+1)= pstMsg->CmdId>>0;

	/**消息长度**/
    *(pu8Data+l_u16DataLen+2)= pstMsg->Len>>8;            
    *(pu8Data+l_u16DataLen+3)= pstMsg->Len>>0;
    l_u16DataLen+=4;
    if(pstMsg->Len+l_u16DataLen>=MSG_TEXT_SIZE)
    {
		return IPCP_FALSE;
	}

	/**消息内容**/
    memcpy(pu8Data+l_u16DataLen,pstMsg->Data,pstMsg->Len);    
    l_u16DataLen+=pstMsg->Len;

	/**消息校验**/
    *(pu8Data+l_u16DataLen+0)=0x55;                    
    l_u16DataLen+=1;

	/**消息结束**/
    *(pu8Data+l_u16DataLen+0)=(unsigned  char)(QUEUE_MSG_END>>8);        
    *(pu8Data+l_u16DataLen+1)=(unsigned  char)(QUEUE_MSG_END);    
    l_u16DataLen+=2;
    
    l_stMsg.MsgLen=l_u16DataLen;
    return IPCP_Internal_Msg_Send(&l_stMsg,l_u16DataLen);
}


/**接收一条命令**/
int  IPCP_Arch_Msg_Recv(int s32ModuleAddr,ARCH_MSG_S *pstMsg)
{
	int l_s32Ret = 0;
	int l_s32ReadLen = 0;
	l_s32Ret = IPCP_Internal_RecieveData(s32ModuleAddr,&l_s32ReadLen);
	if(0==l_s32Ret)
	{
		if(l_s32ReadLen>0)
		{
			memcpy((unsigned char *)pstMsg,(unsigned char *)gs_as8RecvData,l_s32ReadLen);
			l_s32Ret = l_s32ReadLen;
		}else
		{
			l_s32Ret = 0;
		}
	}else
	{
		l_s32Ret = -1;
	}
	
	return l_s32Ret;
}

/******************************************************** 
Function:	 IPCP_Internal_RecieveData  
Description: 读取数据,确保读取到的命令消息是完整的	
Input:
	s32ModuleAddr 预分配给该进程的地址
	*ps32Datalen  用来返回读取到的数据长度
OutPut: 
	*ps32Datalen  读取到的数据长度
Return: 
	0 ： 正常
	非0：异常
Others:
	1.检测到发送端如果已经关闭或是退出，则接收端也关闭该连接	
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
int IPCP_Internal_RecieveData(int s32ModuleAddr, int *ps32Datalen)
{
	int l_s32Res = 0;
	int l_s32MsgLen = 0;
	int l_s32ReadLen = 0;
	fd_set l_stfds;
	struct timeval timeout={0,50};

	if(NULL==ps32Datalen)
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		*ps32Datalen = 0;
		return -1;
	}

	if(gs_s32SocketId<=0)
	{
		l_s32Res = IPCP_Internal_InitSocket(s32ModuleAddr);	
		if(l_s32Res!=0)
		{
			printf("%s %d Socket Init Error \n",__FUNCTION__,__LINE__);
			*ps32Datalen = 0;
			return -2;
		}
	}

		/**判断客户端是否已经关闭**/
	FD_ZERO(&l_stfds);
	FD_SET(gs_s32SocketId,&l_stfds);
	l_s32Res = select(gs_s32SocketId+1,&l_stfds,NULL,NULL,&timeout);
	if(l_s32Res > 0)
	{
		/**长度4字节 + 0x5a5a = 6字节**/
		l_s32Res=read(gs_s32SocketId,gs_as8RecvData,6);
		if(6==l_s32Res)
		{
			l_s32MsgLen = 0;
			if((0xa5==gs_as8RecvData[4])&&(0xa5==gs_as8RecvData[5]))
			{
				l_s32MsgLen |= gs_as8RecvData[0];
				l_s32MsgLen |= gs_as8RecvData[1] << 8;
				l_s32MsgLen |= gs_as8RecvData[2] << 16;
				l_s32MsgLen |= gs_as8RecvData[3] << 24;

				l_s32ReadLen = l_s32MsgLen - 6;
				if(l_s32ReadLen<=0)
				{
					printf("msg len error len = %d \n",l_s32MsgLen);
				}

				l_s32Res=read(gs_s32SocketId,&gs_as8RecvData[6],l_s32ReadLen);
				if(l_s32ReadLen==l_s32Res)
				{
					*ps32Datalen = l_s32ReadLen + 6;
					return 0;
				}else
				{
					if(errno == EINTR)
					{
						printf("Interrupted by the signal!\n");
						*ps32Datalen = 0;
						return -3;
					} 
					else
					{
						/**连接的对方已经关闭**/
						printf("%s %d error!,the sockfd maybe closed fd =%d !\n",
						__FUNCTION__,__LINE__,gs_s32SocketId);
						close(gs_s32SocketId);
						*ps32Datalen = 0;
						return -4;
					}
				}
							
			}
		}else
		{
			if(errno == EINTR)
			{
				printf("Interrupted by the signal!\n");
				*ps32Datalen = 0;
				return -3;
			} 
			else
			{
				/**连接的对方已经关闭**/
				printf("%s %d error!,the sockfd maybe closed fd =%d !\n",
				__FUNCTION__,__LINE__,gs_s32SocketId);
				close(gs_s32SocketId);
				*ps32Datalen = 0;
				return -4;
			}
		}
		
	}else if(0==l_s32Res)
	{
		//printf("%s %d the timeout! content normal\n",__FUNCTION__,__LINE__);
		*ps32Datalen = 0;
		return 0;
	}else if (errno == EINTR)
	{
		printf("Interrupted by the signal!\n");
		*ps32Datalen = 0;
		return -5;
	}else 
	{
		printf("select error!,try again!\n");
		*ps32Datalen = 0;
		return -6;
	}

	return 0;
}

/******************************************************** 
Function: IPCP_Internal_Msg_Send  
Description: 
	写数据到socket
Input:	
	*pstMsg
	s32Len
OutPut: none
Return:
	0: 全部发送成功
	1：缓存已满，发送失败，需要重新再发送
Others:
	这里设置的是连续发送3次失败后丢弃该包数据发送
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
static int IPCP_Internal_Msg_Send(ARCH_MSG_S *pstMsg,int s32Len)
{
	int l_s32Res = 0;
	int l_s32Count = 3;
	l_s32Res = write(gs_s32SocketId, (unsigned char *)pstMsg, s32Len);
	if(l_s32Res!=s32Len)
	{	
		while((l_s32Count--)>0)
		{
			usleep(1000*100);
			l_s32Res = write(gs_s32SocketId, (unsigned char *)pstMsg, s32Len);
			if(l_s32Res==s32Len)
			{
				return 0;
			}

			if(EAGAIN!=errno)
			{
				printf("Send Msg Error l_s32Res = %d errno = %d \n",l_s32Res,errno);			
				return -1;
			}
		}
		printf("Try again, msg send throw away!! \n");
		return 1;
	}

	return 0;
}

static int IPCP_Internal_NeedMoreData(unsigned char *pu8Addr,int s32Len,int *ps32NeedLen)
{
	return 0;
}


/******************************************************** 
Function: IPCP_Internal_TcpCheckClientPort  
Description: 
	查询为TCP设备预分配好的IP端口号
Input:
	s32ModuleAddr： 模块地址
OutPut:none
Return:
	小于0：异常
	大于0：TCP客户端预分配的端口号
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
static int IPCP_Internal_TcpCheckClientPort(int s32ModuleAddr)
{
	int i = 0;
	int l_s32Ret = -1;
	
	if((s32ModuleAddr<DEV_CLIENT_MIN_ADDR)&&(s32ModuleAddr>DEV_CLIENT_MAX_ADDR))
	{
		printf("%s %d input para error\n",__FUNCTION__,__LINE__);
		return -1;
	}

	for(i=0;i<TCP_SERVER_LISTEN_MAX_NUM;i++)
	{
		if(s32ModuleAddr==gc_as32DeviceModuleMap[i][2])
		{
			l_s32Ret = gc_as32DeviceModuleMap[i][1];
			break;
		}
	}

	return l_s32Ret;
	
}



/******************************************************** 
Function:	 IPCP_Internal_TcpClientConnetToServer  
Description: 网络设备通过TCP连接到服务器
Input:	s32ModuleAddr：TCP客户端模块地址
OutPut: none
Return:
	小于0：异常
	等于0：成功
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
static int IPCP_Internal_TcpClientConnetToServer(int s32ModuleAddr)
{
	int l_s32Len = 0;
	int l_s32Res = 0;
	int l_s32Port = 0;
	int l_s32SockFd = 0;
	struct sockaddr_in l_stServerAddr;
	struct sockaddr_in l_stClientAddr;

	int ret = 0;
	int optval;
	int optlen = sizeof(int);

	if((l_s32SockFd = socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket");
		if(l_s32SockFd>0);
		close(l_s32SockFd);
		l_s32SockFd = 0;
		return -1;
	}else
	{
		//gs_s32SocketId = l_s32SockFd;
	}
	
	ret = getsockopt(l_s32SockFd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	printf("%s %d ret = %d\n", __FUNCTION__,__LINE__,ret);
	printf("SO_RCVBUF = %d\n", optval);
	printf("SO_RCVBUF = %d\n", optlen);

	ret = getsockopt(l_s32SockFd, SOL_SOCKET, SO_SNDBUF , &optval, &optlen);
	printf("%s %d ret = %d\n", __FUNCTION__,__LINE__,ret);
	printf("SO_SNDBUF = %d\n", optval);
	printf("SO_SNDBUF = %d\n", optlen);



	l_s32Port = IPCP_Internal_TcpCheckClientPort(s32ModuleAddr);
	if(l_s32Port<=0)
	{
		printf("%s %d tcp addr = %d Port = %d error\n",__FUNCTION__,__LINE__,
			s32ModuleAddr,l_s32Port);
		return -2;
	}

	/**显示绑定客户端**/
	l_stClientAddr.sin_family = AF_INET;    
	l_stClientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	l_stClientAddr.sin_port = htons(l_s32Port); 
    l_s32Len = sizeof(l_stClientAddr);
    if(bind(l_s32SockFd, (struct sockaddr *)&l_stClientAddr, l_s32Len) < 0) 
	{  
		printf("blind port l_s32Port = %d  error\n",l_s32Port);
        perror("bind error");
		if(l_s32SockFd>0)
		{
			close(l_s32SockFd);
			l_s32SockFd = 0;
		}
		return -3;
    } 
	
	/**配置server_address**/
	l_stServerAddr.sin_family = AF_INET;
	l_stServerAddr.sin_port = htons(TCP_SERVER_PORT); 	
	inet_pton(AF_INET, TCP_SERVER_IP, &l_stServerAddr.sin_addr);
	
	l_s32Len = sizeof(l_stServerAddr);
	l_s32Res = connect(l_s32SockFd, (struct sockaddr *)&l_stServerAddr, l_s32Len);
	if(l_s32Res == -1) 
	{
		printf("ensure the server is up\n");
		perror("connect");
		if(l_s32SockFd>0)
		{
			close(l_s32SockFd);
			l_s32SockFd = 0;
		}
		return -4;
	}

	gs_s32SocketId = l_s32SockFd;


	return 0;
	
}

/******************************************************** 
Function:	 IPCP_Internal_UnixDomainConnetToServer  
Description: 本地模块，通过Unix Domain socket 连接到服务端
Input:	s32ModuleAddr ：本地模块地址
OutPut: none
Return: 0 成功；非0 异常；
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
static int IPCP_Internal_UnixDomainConnetToServer(int s32ModuleAddr)
{
	int l_s32Len = 0;
	int l_s32Res = 0;
	int l_s32SockFd = 0;
	struct sockaddr_un l_stServerAddr;
	struct sockaddr_un l_stClientAddr;
	char l_as8ClientPath [64] = {0};

	int ret = 0;
	int optval;
	int optlen = sizeof(int);


	if((s32ModuleAddr<LOCAL_CLIENT_1_ADDR)&&(s32ModuleAddr>LOCAL_CLIENT_MAX_ADDR))
	{
		printf("%s %d input para error %d \n",__FUNCTION__,__LINE__,s32ModuleAddr);
		return -1;
	}

	snprintf(l_as8ClientPath,sizeof(l_as8ClientPath),"%s%d",CLIENT_PACHT,s32ModuleAddr);

	/**创建socket，指定通信协议为AF_UNIX,数据方式SOCK_STREAM**/
	if((l_s32SockFd = socket(AF_UNIX, SOCK_STREAM|SOCK_NONBLOCK, 0))==-1)
	{
		if(l_s32SockFd>0)
		{
			close(l_s32SockFd);
			l_s32SockFd = 0;
		}
		perror("socket");
		exit(EXIT_FAILURE);
	}

	ret = getsockopt(l_s32SockFd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	printf("%s %d ret = %d\n", __FUNCTION__,__LINE__,ret);
	printf("SO_RCVBUF = %d\n", optval);
	printf("SO_RCVBUF = %d\n", optlen);

	ret = getsockopt(l_s32SockFd, SOL_SOCKET, SO_SNDBUF , &optval, &optlen);
	printf("%s %d ret = %d\n", __FUNCTION__,__LINE__,ret);
	printf("SO_SNDBUF = %d\n", optval);
	printf("SO_SNDBUF = %d\n", optlen);

	
	/**一般显式调用bind函数，以便服务器区分不同客户端  
	它也可以不用bind,直接连接**/ 
    memset(&l_stClientAddr, 0, sizeof(l_stClientAddr));  
    l_stClientAddr.sun_family = AF_UNIX;  
    strcpy(l_stClientAddr.sun_path, l_as8ClientPath);  
    l_s32Len = sizeof(l_stClientAddr);
    unlink(l_as8ClientPath);  
    if(bind(l_s32SockFd, (struct sockaddr *)&l_stClientAddr, l_s32Len) < 0) 
	{  
        perror("bind error");
		if(l_s32SockFd>0)
		{
			close(l_s32SockFd);
			l_s32SockFd = 0;
		}
        return -1;
    } 
	
	/**配置server_address**/
	l_stServerAddr.sun_family = AF_UNIX;
	strcpy(l_stServerAddr.sun_path, SERVER_PATH);
	l_s32Len = sizeof(l_stServerAddr);
 
	l_s32Res = connect(l_s32SockFd, (struct sockaddr *)&l_stServerAddr, l_s32Len);
	if(l_s32Res == -1) 
	{
		printf("ensure the server is up\n");
		perror("connect");
		if(l_s32SockFd>0)
		{
			close(l_s32SockFd);
			l_s32SockFd = 0;
		}
		return -1;
	}

	gs_s32SocketId = l_s32SockFd;
	printf("unix domain addr:%d connet success \n",s32ModuleAddr);

	return 0;

}

/******************************************************** 
Function:	 IPCP_Internal_CheckNetType  
Description: 判断客户端是需要Unix domain还是TCP连接
Input:	s32ModuleAddr ：本地模块地址
OutPut: none
Return:
	小于0：异常
	大于0：类型 
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
static int IPCP_Internal_CheckNetType(int s32ModuleAddr)
{
	if((s32ModuleAddr<CLIENT_MIN_ADDR)&&(s32ModuleAddr>CLIENT_MAX_ADDR))
	{
		printf("%s %d input para error s32ModuleAddr = %d \n",__FUNCTION__,__LINE__,s32ModuleAddr);
		return -1;
	}else if((s32ModuleAddr>=CLIENT_MIN_ADDR)&&(s32ModuleAddr<=LOCAL_CLIENT_MAX_ADDR))
	{
		return UNIX_DOMAIN;
	}else if((s32ModuleAddr>=DEV_CLIENT_MIN_ADDR)&&(s32ModuleAddr<=CLIENT_MAX_ADDR))
	{
		return TCP_SOCKET;
	}else
	{
		printf("Client Module Addr Error %d \n",s32ModuleAddr);
		return -1;
	}
}

/******************************************************** 
Function:	 IPCP_Internal_InitSocket  
Description: 客户端连接服务网络初始化接口	
Input:	s32ModuleAddr:模块地址
OutPut: none
Return: 0 成功；非0 异常；
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
static int IPCP_Internal_InitSocket(int s32ModuleAddr)
{
	int l_s32Ret = 0;
	int l_s32Res = 0;

	if(gs_s32SocketId>0)
	{
		printf("Socket already start now \n");
		return 1;
	};

	l_s32Ret = IPCP_Internal_CheckNetType(s32ModuleAddr);
	switch(l_s32Ret)
	{
		case UNIX_DOMAIN:
		{
			l_s32Res = IPCP_Internal_UnixDomainConnetToServer(s32ModuleAddr);
			break;
		}

		case TCP_SOCKET:
		{
			l_s32Res = IPCP_Internal_TcpClientConnetToServer(s32ModuleAddr);
			break;
		}

		default:
		{
			l_s32Res = -1;
			break;
		}

	}

	return l_s32Res;
}

