/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: server.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description:
	socket进程间通信服务端，主要作用为接收客户端链接以及
	客户端数据路由
*Date:	   2019-08-03
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
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "ipc_interface.h"
#include "ipc_msgstruct.h"
#include "ipc_common.h"

/**客户端网络连接表，对应客户端模块编号**/
int g_as32ClientSocketId[CLIENT_MAX_ADDR] = {0};

/**TCP服务端的ID**/
int g_s32TcpServerSocketId = 0;

/**线程状态，用来控制线程退出**/
char g_s8UnixConnectState = IPCP_FALSE;
char g_s8TcpConnectState  = IPCP_FALSE;
char g_s8ReadWriteState   = IPCP_FALSE;

typedef struct MsqAnalyze
{
	int s32Len;				/**消息长度**/
	unsigned char *pu8Msg;  /**消息地址**/
}MSG_ANALYZE_S;

/**********************************
linux ctrl + C 会产生 SIGINT信号
接收到SIGINT 信号进入该函数
**********************************/
void stop(int signo)
{
	int i = 0;
	printf(" stop \n");

	g_s8UnixConnectState = IPCP_FALSE;
	g_s8TcpConnectState  = IPCP_FALSE;
	g_s8ReadWriteState   = IPCP_FALSE;

	usleep(20000);
	if(g_s32TcpServerSocketId>0)
	{
		close(g_s32TcpServerSocketId);
		g_s32TcpServerSocketId = 0;
	}
	
	for(i=0;i<CLIENT_MAX_ADDR;i++)
	{
		if(g_as32ClientSocketId[i]>0)
		{
			close(g_as32ClientSocketId[i]);
			g_as32ClientSocketId[i] = 0;
		}
	}
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

int IPCP_Server_NeedMoreData(unsigned char *pu8Addr,int s32Len,int *ps32NeedLen)
{
	int l_s32Res = 0;
	unsigned int l_s32Pos = 0;
	unsigned int l_s32MsgLen = 0;
	unsigned int l_s32Temp = 0;
	unsigned char l_au8Buff[4] = {0};
	unsigned char * l_pu8Addr = NULL;
	if((NULL==pu8Addr)||(s32Len<=0))
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}

	l_pu8Addr = pu8Addr;
	l_s32Pos  = 0;

	while(l_s32Pos<=s32Len)
	{
		
		/**消息长度接收完整**/
		if((l_s32Pos+4)<=s32Len)
		{	
			l_s32MsgLen = 0;
				
			l_s32Temp = l_pu8Addr[l_s32Pos];
			l_s32MsgLen |= l_s32Temp;

			l_s32Temp = l_pu8Addr[l_s32Pos+1];
			l_s32MsgLen |= l_s32Temp << 8;

			l_s32Temp = l_pu8Addr[l_s32Pos+2];
			l_s32MsgLen |= l_s32Temp << 16;

			l_s32Temp = l_pu8Addr[l_s32Pos+3];
			l_s32MsgLen |= l_s32Temp << 24;
			
			if((l_s32MsgLen+l_s32Pos)>s32Len)
			{
				*ps32NeedLen = (l_s32MsgLen+l_s32Pos) - s32Len;
				return 0;
			}
			
		}else
		{
			/**消息长度接收不完整**/
			*ps32NeedLen = s32Len - l_s32Pos;
			   return 1;	
		}
		l_s32Pos += l_s32MsgLen;	
	}	
}

/******************************************************** 
Function:	 IPCP_Server_RecieveData  
Description: 读取数据,确保读取到的命令消息是完整的	
Input:	
	*s32SocketId 网络连接ID;
	**ppu8DataAddr 输入时需要为空;
	*ps32Datalen 接收数据的长度，输入时为0；
OutPut: 
	*s32SocketId 发送端关闭时，该值设置为0;
	**ppu8DataAddr 输出数据存放的地址
	*ps32Datalen 输出实际接收数据的长度
Return: 
	0 ：正常
	小于0 : 异常
Others:
	1.检测到发送端如果已经关闭或是退出，则接收端也关闭该连接
	
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
int IPCP_Server_RecieveData(int *s32SocketId,unsigned char **ppu8DataAddr,int *ps32Datalen)
{
	int l_s32Res       = 0;
	int l_s32BuffLen   = 0;
	int l_s32BufMaxLen = 0;
	int l_s32SocketId  = 0;
	int l_s32NeedLen   = 0;
	int l_s32MsgLen    = 0;
	int l_s32Pos	   = 0;
	
	fd_set l_stfds;
	struct timeval timeout={0,50};
	
	static unsigned char *ls_pu8DataAddr = NULL;

	if((NULL==s32SocketId)||(*s32SocketId<=0)||(NULL==ps32Datalen))
	{
		printf("%s %d input para error \n",__FUNCTION__,__LINE__);
		return -1;
	}

	l_s32SocketId  = *s32SocketId;
	l_s32BuffLen   = MSG_TEXT_SIZE*1;
	l_s32BufMaxLen = l_s32BuffLen + MSG_TEXT_SIZE;
	if(NULL==ls_pu8DataAddr)
	{
		ls_pu8DataAddr = malloc(l_s32BuffLen);
		if(NULL==ls_pu8DataAddr)
		{
			printf("%s %d malloc error \n",__FUNCTION__,__LINE__);
			return -2;
		};
	};

	memset(ls_pu8DataAddr,0,l_s32BuffLen);
	
	/**判断客户端是否已经关闭**/
	FD_ZERO(&l_stfds);
	FD_SET(l_s32SocketId,&l_stfds);
	l_s32Res = select(l_s32SocketId+1,&l_stfds,NULL,NULL,&timeout);
	if(l_s32Res > 0)
	{
		l_s32Res=read(l_s32SocketId,ls_pu8DataAddr,l_s32BuffLen);
		if((l_s32Res>0)&&(l_s32Res<l_s32BuffLen))
		{
			/**命令消息完整**/
			*ppu8DataAddr = ls_pu8DataAddr;
			*ps32Datalen  = l_s32Res;
			return 0;
		}else if(l_s32BuffLen==l_s32Res)
		{
			/**命令包不完整，需要再读取部分数据**/
			l_s32NeedLen = 0;
			l_s32Res = IPCP_Server_NeedMoreData(ls_pu8DataAddr,l_s32BuffLen,&l_s32NeedLen);
			if((0==l_s32Res)&&(l_s32NeedLen>0)&&(l_s32NeedLen<MSG_TEXT_SIZE))
			{
				l_s32Res=read(l_s32SocketId,ls_pu8DataAddr+l_s32BuffLen,l_s32NeedLen);
				if(l_s32Res==l_s32NeedLen)
				{
					*ppu8DataAddr = ls_pu8DataAddr;
					*ps32Datalen  = l_s32Res;
					return 0;
				}else
				{
					printf("%s %d read data error l_s32Res= %d  \n",__FUNCTION__,__LINE__,l_s32Res);
					return -1;
				}

			}else if((1==l_s32Res)&&(l_s32NeedLen>0)&&(l_s32NeedLen<4))
			{
				l_s32Res=read(l_s32SocketId,ls_pu8DataAddr+l_s32BuffLen,l_s32NeedLen);
				if(l_s32Res==l_s32NeedLen)
				{
					l_s32MsgLen = 0;
					l_s32MsgLen |= *(ls_pu8DataAddr + l_s32BuffLen + l_s32NeedLen) <<24;
					l_s32MsgLen |= *(ls_pu8DataAddr + l_s32BuffLen + l_s32NeedLen -1) <<16;
					l_s32MsgLen |= *(ls_pu8DataAddr + l_s32BuffLen + l_s32NeedLen -2) <<8;
					l_s32MsgLen |= *(ls_pu8DataAddr + l_s32BuffLen + l_s32NeedLen -3);
					l_s32Pos = l_s32BuffLen+(4-l_s32NeedLen);
					l_s32Res=read(l_s32SocketId,ls_pu8DataAddr+l_s32Pos,l_s32MsgLen);
					if(l_s32Res==l_s32MsgLen)
					{
						*ppu8DataAddr = ls_pu8DataAddr;
						*ps32Datalen  = l_s32Pos + l_s32MsgLen;
						return 0;
					}else
					{
					
					}
				}
				printf("%s %d biao debug\n",__FUNCTION__,__LINE__);
			}else
			{
				printf("%s %d biao debug\n",__FUNCTION__,__LINE__);
			}
			
		}else
		{
			if(errno == EINTR)
			{
				printf("Interrupted by the signal!\n");
				return -3;
			} 
			else
			{
				/**连接的对方已经关闭**/
				printf("%s %d error!,the sockfd maybe closed fd =%d !\n",
					__FUNCTION__,__LINE__,l_s32SocketId);
				close(l_s32SocketId);
				*s32SocketId = 0;
				return -4;
			}
		}
		
	}else if(0==l_s32Res)
	{
		//printf("%s %d the timeout! content normal\n",__FUNCTION__,__LINE__);
		return 0;
	}else if (errno == EINTR)
	{
		printf("Interrupted by the signal!\n");
		return -5;
	}else 
	{
		printf("select error!,try again!\n");
		return -6;
	}

	return 0;
}


/******************************************************** 
Function:	 IPCP_Server_Router_MsgPackAnalyze  
Description: 
	解析数据包中的所有命令消息
Input:	*pstMsgExpAnalyze
OutPut: *pstMsgExpAnalyze
Return: 0 成功；非0 异常；
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
int IPCP_Server_Router_MsgPackAnalyze(ARCH_MSG_S *pstMsgPack)
{
	int l_s32Res;
	int l_s32TargAddr = 0;
	int l_s32SendLen  = 0;

	fd_set l_stfds;
	struct timeval timeout={1,0};

	if((NULL==pstMsgPack)||(pstMsgPack->MsgLen<=0)||(NULL==pstMsgPack->SomeText))
	{
		printf("%s %d input para error MsgLen = %d \n",
			__FUNCTION__,__LINE__,pstMsgPack->MsgLen);
		return IPCP_FALSE;
	}

	/**获取将要发送的客户端**/
	l_s32TargAddr = IPCP_Arch_Msg_AnalyzeGetTargAddr(pstMsgPack);
	if((l_s32TargAddr<0)||(l_s32TargAddr>CLIENT_MAX_ADDR))
	{
		printf("%s %d client addr error \n",__FUNCTION__,__LINE__);
		return IPCP_FALSE;
	}

	//printf("biao debug msg send to addr = %d \n",l_s32TargAddr);
	
	/**判断客户端是否已经关闭**/
	if(g_as32ClientSocketId[l_s32TargAddr]>0)
	{
		FD_ZERO(&l_stfds);
		FD_SET(g_as32ClientSocketId[l_s32TargAddr],&l_stfds);
		l_s32Res = select(g_as32ClientSocketId[l_s32TargAddr]+1,&l_stfds,NULL,NULL,&timeout);

		if(l_s32Res > 0)
		{

			/**需要判断是否是广播**/
			/**
			need to do ......
			**/
			l_s32SendLen=write(g_as32ClientSocketId[l_s32TargAddr],\
				(char *)pstMsgPack,pstMsgPack->MsgLen);
			if(l_s32SendLen!=l_s32SendLen)
			{
				printf("server write data error \n");
				return IPCP_FALSE;
			}
		}else if(0==l_s32Res) 
		{
			//printf("%s %d the timeout! content normal l_s32TargAddr = %d  id = %d \n",
			//	__FUNCTION__,__LINE__,l_s32TargAddr,g_as32ClientSocketId[l_s32TargAddr]);
		}else if (errno==EINTR)
		{
			printf("%s %d Interrupted by the signal!  l_s32TargAddr = %d id = %d \n",
			   __FUNCTION__,__LINE__,l_s32TargAddr,g_as32ClientSocketId[l_s32TargAddr]);
		} else 
		{
			printf("%s %d select error!,try again!    l_s32TargAddr = %d id = %d ",
				__FUNCTION__,__LINE__,l_s32TargAddr,g_as32ClientSocketId[l_s32TargAddr]);
		}
	}
	return 0;
}

/******************************************************** 
Function:	 IPCP_Server_Router_MsgAnalyze  
Description: 
	从socket中接收到的数据包，里面的命令消息是完整的，但也有
	可能是包含了多条命令消息。
Input:	*pstMsgExpAnalyze
OutPut: *pstMsgExpAnalyze
Return: 0 成功；非0 异常；
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
int IPCP_Server_Router_MsgAnalyze(MSG_ANALYZE_S 			pstMgsAnalyze)
{
    int l_s32Ret = 0;
	int l_s32Len = 0;
	ARCH_MSG_S *l_pstMsg = NULL;

	if((NULL==pstMgsAnalyze.pu8Msg)||(pstMgsAnalyze.s32Len<=0))
	{
		printf("%s %d input para error\n",__FUNCTION__,__LINE__);
		return IPCP_FALSE;
	}else
	{
		l_pstMsg = (ARCH_MSG_S*)pstMgsAnalyze.pu8Msg;
	}
	
	l_s32Len = 0;
	while(l_s32Len < pstMgsAnalyze.s32Len)
	{
		l_pstMsg =(ARCH_MSG_S*)(pstMgsAnalyze.pu8Msg + l_s32Len);
		IPCP_Server_Router_MsgPackAnalyze(l_pstMsg);
		l_s32Len +=l_pstMsg->MsgLen; 
	}
	return 0;
}


/******************************************************** 
Function:	 IPCP_Server_MatchTcpClientBySocket  
Description: 
	通过Ip和端口判断属于哪个网络设备的哪个客户端模块
Input:	
	u32Port: TCP 端口
	*ps8IpStr: 字符串类型的IP地址
OutPut: 
Return: 客户端模块地址，如果小于0表示匹配失败
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
int IPCP_Server_MatchTcpClientBySocket(int s32Port,char *ps8IpStr)
{
	int l_s32StartAddr = 0;
	int l_s32ClientNo = 0; 
	if((NULL==ps8IpStr)||(s32Port<TCP_DEVICE_CLIENT0_POART)||\
		(s32Port>TCP_DEVICE_CLIENT_MAX_POART))
	{
		printf("%s %d :input IP string is NULL \n",__FUNCTION__,__LINE__);
		return -1;
	}

	if(0==strcmp(TCP_CLIENT_DEVICE1_IP,ps8IpStr))
	{
		l_s32StartAddr = DEV1_CLIENT_0_ADDR;
	}else if(0==strcmp(TCP_CLIENT_DEVICE2_IP,ps8IpStr))
	{
		l_s32StartAddr = DEV2_CLIENT_0_ADDR;
	}else
	{
		l_s32StartAddr = -1;
	}

	if(l_s32StartAddr > 0)
	{
		l_s32ClientNo = l_s32StartAddr + (s32Port - TCP_DEVICE_CLIENT0_POART);
	}else
	{
		l_s32ClientNo = -1;
	}

	return l_s32ClientNo;
}

/******************************************************** 
Function:	 IPCP_Server_ReadWriteFunc  
Description: 
	服务端进行数据的接收和发送，以实现数据的路由功能
Input:	*arg
OutPut: none
Return: void *
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
void* IPCP_Server_ReadWriteFunc(void* arg)
{
	int i = 0;
	int l_s32Res = 0;
	int l_s32Len = 0;
	unsigned char *l_pu8Recv = NULL;
	MSG_ANALYZE_S  l_stMsgAnalyze = {0};

	while(IPCP_TRUE==g_s8ReadWriteState)
	{
		for(i=0;i<CLIENT_MAX_ADDR;i++)
		{
			if(0==g_as32ClientSocketId[i])
			{
				continue;
			}
			l_s32Len  = 0;
			l_s32Res = IPCP_Server_RecieveData(&g_as32ClientSocketId[i],&l_pu8Recv,&l_s32Len);
			if((0==l_s32Res)&&(g_as32ClientSocketId[i]>0)&&(l_s32Len>0)&&(NULL!=l_pu8Recv))
			{
				l_stMsgAnalyze.pu8Msg = l_pu8Recv;
				l_stMsgAnalyze.s32Len = l_s32Len;
				IPCP_Server_Router_MsgAnalyze(l_stMsgAnalyze);
			}else if((0==l_s32Res)&&(0==l_s32Len))
			{
				
			}else
			{
				if(0!=g_as32ClientSocketId[i])
				{
					g_as32ClientSocketId[i] = 0;
				}
				printf("%s %d receive data error l_s32Res = %d \n",__FUNCTION__,__LINE__,l_s32Res);
			}
		};
		usleep(1000);
	}

ERROR:

	if(NULL!=l_pu8Recv)
	{
		free(l_pu8Recv);
		l_pu8Recv = NULL;
	};
	
}


/******************************************************** 
Function:	 IPCP_Server_UnixDomainNetFunc  
Description: 
	监听本地客户端模块连接，并把对应模块的连接添加到
	网络连接表中。
Input:	*arg
OutPut: none
Return: void *
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
void* IPCP_Server_UnixDomainNetFunc(void* arg)
{
	int i = 0;
	int l_s32ServerSockFd = 0;
	int l_s32ClientSockFd = 0;
	int l_s32ServerLen = 0;
	int l_s32ClientLen = 0;
	int l_s32ClientId = 0;

	int ret = 0;
	int optval;
	int optlen = sizeof(int);

	struct sockaddr_un l_stServerAddr;
	struct sockaddr_un l_stClientAddr;
		
	/**解除原有server_socket对象链接**/
	unlink(SERVER_PATH);	
	
	/**创建socket，指定通信协议为AF_UNIX,数据方式SOCK_STREAM**/
	l_s32ServerSockFd = socket(AF_UNIX, SOCK_STREAM|SOCK_NONBLOCK, 0);

	ret = getsockopt(l_s32ServerSockFd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	printf("Unix Domain SO_RCVBUF = %d\n", optval);

	ret = getsockopt(l_s32ServerSockFd, SOL_SOCKET, SO_SNDBUF , &optval, &optlen);
	printf("Unix Domain SO_SNDBUF = %d\n", optval);
	
	/**配置l_stServerAddr**/
	l_stServerAddr.sun_family = AF_UNIX;
	strcpy(l_stServerAddr.sun_path, SERVER_PATH);
	l_s32ServerLen = sizeof(l_stServerAddr);
	
	bind(l_s32ServerSockFd, (struct sockaddr *)&l_stServerAddr, l_s32ServerLen);
 
	/**最大监听10个客户端**/
	listen(l_s32ServerSockFd, 10);
	
	printf("server waiting for unix domain client connect\n");
	l_s32ClientLen = sizeof(l_stClientAddr);
	
	/**接收客户端的链接**/
	while(IPCP_TRUE==g_s8UnixConnectState)
	{
		l_s32ClientSockFd = 0;
		l_s32ClientSockFd = accept(l_s32ServerSockFd,(struct sockaddr *)&l_stClientAddr,\
			(socklen_t *)&l_s32ClientLen);
		
		if(-1==l_s32ClientSockFd)
		{
			//printf("accept socket error\n");
			usleep(100000);
			continue;
		}

		/**将客该SockFd与对应的客户端建立映射关系**/
		printf("unix domain socket patch = %s \n",l_stClientAddr.sun_path);
		sscanf(&l_stClientAddr.sun_path[strlen(CLIENT_PACHT)],"%d",&l_s32ClientId);
		if((l_s32ClientId>0)&&(l_s32ClientId<MAX_UDS_CLIENT_NUM))
		{
			g_as32ClientSocketId[l_s32ClientId] = l_s32ClientSockFd;
			printf("Process =%d ,SocketId = %d \n",l_s32ClientId,l_s32ClientSockFd);
		}else
		{
			printf("client Process id is error ! \n");
		}
    }

	if(l_s32ServerSockFd>0)
	{
		close(l_s32ServerSockFd);
		l_s32ServerSockFd = 0;
	}
	
	for(i=LOCAL_CLIENT_1_ADDR;i<LOCAL_CLIENT_MAX_ADDR;i++)
	{
		if(g_as32ClientSocketId[i]>0)
		{
			close(g_as32ClientSocketId[i]);
			g_as32ClientSocketId[i] = 0;
		}
	}
	unlink(SERVER_PATH);
}

/******************************************************** 
Function:	 IPCP_Server_TcpConnectFunc  
Description: 	
	监听远程模块连接，并把对应模块的连接添加到
	网络连接表中。该连接是TCP连接
Input:	*arg
OutPut: none
Return: void *
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
void* IPCP_Server_TcpConnectFunc(void* arg)
{
	int i = 0;
	int l_s32Port = 0;
	int l_s32AddLen = 0;
	int l_s32ClientNo = 0;
	int l_s32ServerSockFd = 0;
	int l_s32ClientSockFd = 0;

	struct sockaddr_in l_stServerAddr;  
	struct sockaddr_in l_stClientAddr;  
	
	char l_as8Tmp[64] = {0};

	int ret = 0;
	int optval;
	int optlen = sizeof(int);


	/**建立非阻塞TCP连接**/
	if((l_s32ServerSockFd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0)) == -1)
	{
		printf("create socket error %s %d \n",__FUNCTION__,__LINE__);
	}else
	{
		g_s32TcpServerSocketId = l_s32ServerSockFd; 
	}

	ret = getsockopt(l_s32ServerSockFd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
	printf("TCP SO_RCVBUF = %d\n", optval);

	ret = getsockopt(l_s32ServerSockFd, SOL_SOCKET, SO_SNDBUF , &optval, &optlen);
	printf("TCP SO_SNDBUF = %d\n", optval);



	/**设置服务端地址**/
	l_s32AddLen = sizeof(struct sockaddr_in);
	memset(&l_stServerAddr, 0, l_s32AddLen);
	
	/**AF_INET表示 IPv4 Intern 协议**/
	l_stServerAddr.sin_family = AF_INET;    
	
	/**INADDR_ANY 可以监听任意IP**/
	l_stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	/**设置端口**/
	l_stServerAddr.sin_port = htons(TCP_SERVER_PORT); 

	/**绑定地址结构到套接字描述符**/
	if(bind(l_s32ServerSockFd, (struct sockaddr*)&l_stServerAddr, sizeof(l_stServerAddr)) == -1)
	{
		printf("bind socket error %s %d \n",__FUNCTION__,__LINE__);
	}

	/**设置监听队列**/
	if( listen(l_s32ServerSockFd, TCP_SERVER_LISTEN_MAX_NUM) == -1)
	{
		printf("listen socket error %s %d \n",__FUNCTION__,__LINE__);
	}
	
	printf("server waiting for tcp client connect\n");
		
	while(IPCP_TRUE==g_s8TcpConnectState)
	{
		/**接收客户端的连接，这里会阻塞，直到有客户端连接**/
		if((l_s32ClientSockFd = accept(l_s32ServerSockFd,\
			(struct sockaddr*)&l_stClientAddr, &l_s32AddLen)) == -1)
		{
			usleep(100000);
			continue;
		}
		
		l_s32Port = ntohs(l_stClientAddr.sin_port);
		inet_ntop(AF_INET,&l_stClientAddr.sin_addr,l_as8Tmp,sizeof(l_as8Tmp));
		
		printf("sin_port = %u \n",l_s32Port);
		printf("sin_addr = %s \n",l_as8Tmp);

		/**将客该SockFd与对应的客户端建立映射关系**/
		l_s32ClientNo = IPCP_Server_MatchTcpClientBySocket(l_s32Port,l_as8Tmp);
		if((l_s32ClientNo >= 0)&&(l_s32ClientNo<=CLIENT_MAX_ADDR))
		{
			printf("tcp client conent No. = %d  fd = %d \n",l_s32ClientNo,l_s32ClientSockFd);
			g_as32ClientSocketId[l_s32ClientNo] = l_s32ClientSockFd;
		}
		
	}
	if(l_s32ServerSockFd>0)
	{
		close(l_s32ServerSockFd);
		l_s32ServerSockFd = 0;
	}
	
	for(i=DEV_CLIENT_MIN_ADDR;i<DEV_CLIENT_MAX_ADDR;i++)
	{
		if(g_as32ClientSocketId[i]>0)
		{
			close(g_as32ClientSocketId[i]);
			g_as32ClientSocketId[i] = 0;
		}
	}	
}

/******************************************************** 
Function:	 UnixDomainNetFunc  
Description: 服务端入口
Input:	argc;** argv
OutPut: 
Return: 
Others:
Author: Caibiao Lee
Date:	2019-08-03
*********************************************************/
int main(int argc, char** argv)
{
	int l_s32Para;
	pthread_t l_UnixConnectId;
	pthread_t l_TcpConnectId;
	pthread_t l_ReadWriteId;

	/**注册 SIGPIPE信号**/
	signal(SIGPIPE,signal_pipe); 
	
	/**注册SIGINT 信号**/
	signal(SIGINT,stop); 

	/**创建unix domain socket连接线程**/
	if(0==pthread_create(&l_UnixConnectId,NULL,IPCP_Server_UnixDomainNetFunc,&l_s32Para))
	{
		g_s8UnixConnectState = IPCP_TRUE;
		printf("create unix domin socket lpthread success\n");	
	}else
	{
		printf("create unix domin socket lpthread error !!!\n");	
	}
	
	/**创建tcp连接线程**/
	if(0==pthread_create(&l_TcpConnectId,NULL,IPCP_Server_TcpConnectFunc,&l_s32Para))
	{
		g_s8TcpConnectState = IPCP_TRUE;
		printf("create TCP socket lpthread success\n");
	}else
	{
		printf("create TCP socket lpthread error !!!\n");
	}

	/**创建读写线程**/
	if(0==pthread_create(&l_ReadWriteId,NULL,IPCP_Server_ReadWriteFunc,&l_s32Para))
	{
		g_s8ReadWriteState = IPCP_TRUE;
		printf("create read write lpthread seccess\n");
	}else
	{
		printf("create read write lpthread error !!!\n");
	}

	/**阻塞，直到线程退出才能退出**/
	pthread_join(l_UnixConnectId,NULL);
	pthread_join(l_TcpConnectId,NULL);	
	pthread_join(l_ReadWriteId,NULL); 
	
	printf("server logout \n");
	
	return 0;
}

