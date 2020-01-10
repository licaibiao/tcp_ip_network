/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: 01_server_tcp.c
*BlogAddr: caibiao-lee.blog.csdn.net
*Description: TCP 客户端收发数据 
*Date:     2020-01-04
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
	通过read write 函数的返回值和错误码判断对方连接是否已经断开
*History:
***********************************************************/
#include <sys/uio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define SERVER_IP_ADDR		"192.168.1.111"
#define PORT 8888		/* 侦听端口地址 */
#define BACKLOG 2		/* 侦听队列长度 */

void sig_proccess(int signo)
{
	printf("Catch a exit signal\n");
	exit(0);	
}

void sig_pipe(int sign)
{
	printf("Catch a SIGPIPE signal\n");
	
	/* 释放资源 */	
}

/******************************************************** 
Function:	 check_tcp_alive	
Description: 通过TCP_INFO查询网络状态
Input:	s32SocketFd ：服务端接收到客户端连接的ID;
OutPut: none
Return: 0: success，none 0:error
Others: 
Author: Caibiao Lee
Date:	2020-01-04
*********************************************************/
int check_tcp_alive(int s32SocketFd)
{
	while(1)
	{
		printf("alive  s32SocketFd = %d \n",s32SocketFd);
		if(s32SocketFd>0)
		{
			struct tcp_info info;
			int len = sizeof(info);

			getsockopt(s32SocketFd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
			
			printf("info.tcpi_state = %d\n",info.tcpi_state);
			if(info.tcpi_state == TCP_ESTABLISHED)
			{
				printf("connect ok \r\n");
				//return 0;
			}
			else
			{
				printf("connect error\r\n");
				//return -1;
			}
		}
		sleep(1);
		printf("\n\n");
	}
}


/******************************************************** 
Function:	 process_conn_server	
Description: 服务器对客户端的处理
Input:	s32SocketFd ：服务端接收到客户端连接的ID;
OutPut: none
Return: 0: success，none 0:error
Others: 
Author: Caibiao Lee
Date:	2020-01-04
*********************************************************/
void process_conn_server(int s32SocketFd)
{
	int size = 0;
	char buffer[1024];	/* 数据的缓冲区 */
	pid_t pid;	/* 分叉的进行id */
	
	pid = fork();		/* 分叉进程 */
	if( pid == 0 )
	{		
		check_tcp_alive(s32SocketFd);
	}
	
	for(;;)
	{	
		/* 从套接字中读取数据放到缓冲区buffer中 */
		size = read(s32SocketFd, buffer, 1024);	
		if(size==0)
		{/* 没有数据 */
			printf("read size = %d, error %d \n",size,errno);
			//return;	
		}else if(size<0)
		{
			printf("read size = %d, error %d \n",size,errno);
			//return ;
		}else 
		{
			printf("recv data:%s \n",buffer);
			
		}
		memset(buffer,0,sizeof(buffer));	
		/* 构建响应字符，为接收到客户端字节的数量 */
		strcpy(buffer,"I am server");
		size = write(s32SocketFd, buffer, strlen(buffer)+1);/* 发给客户端 */
		if((strlen(buffer)+1)==size)
		{

		}else
		{
			printf("write data error size = %d, errno=%d\n",size,errno);
		//return ;
		}
		sleep(1);
	}	
}

int main(int argc, char *argv[])
{
	int l_s32ServerFd = -1;
	int l_s32ClientrFd = -1;		
	struct sockaddr_in server_addr; /* 服务器地址结构 */
	struct sockaddr_in client_addr;	/* 客户端地址结构 */
	int l_s32Ret = 0;	/* 返回值 */
	pid_t pid;	/* 分叉的进行id */
	
	signal(SIGINT, sig_proccess);
	signal(SIGPIPE, sig_pipe);
	
	
	/* 建立一个流式套接字 */
	l_s32ServerFd = socket(AF_INET, SOCK_STREAM, 0);
	if(l_s32ServerFd < 0)
	{/* 出错 */
		printf("socket error\n");
		return -1;	
	}	
	
	/* 设置服务器地址 */
	bzero(&server_addr, sizeof(server_addr));	/* 清0 */
	server_addr.sin_family = AF_INET;			/* 协议族 */
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);/*服务器IP地址*/
	server_addr.sin_port = htons(PORT);			/* 服务器端口 */
	
	
    /*设置IP地址可以重复绑定*/
	int l_s32UseAddr = 1;
    if(setsockopt(l_s32ServerFd, SOL_SOCKET, SO_REUSEADDR, &l_s32UseAddr, sizeof(int)) < 0)
    {
        printf("%s %d\tsetsockopt error! Error code: %d，Error message: %s\n", 
            __FUNCTION__, __LINE__, errno, strerror(errno));
        return -2;
    }

	/* 绑定地址结构到套接字描述符 */
	l_s32Ret = bind(l_s32ServerFd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(l_s32Ret < 0)
	{/* 出错 */
		printf("bind error\n");
		return -1;	
	}
	
	/* 设置侦听 */
	l_s32Ret = listen(l_s32ServerFd, BACKLOG);
	if(l_s32Ret < 0)
	{/* 出错 */
		printf("listen error\n");
		return -1;	
	}
	
	/* 主循环过程 */
	for(;;)
	{
		int addrlen = sizeof(struct sockaddr);
		/* 接收客户端连接 */
		l_s32ClientrFd = accept(l_s32ServerFd, (struct sockaddr*)&client_addr, &addrlen);
		if(l_s32ClientrFd < 0)
		{	/* 出错 */
			continue;	/* 结束本次循环 */
		}	
		
		/* 建立一个新的进程处理到来的连接 */
		pid = fork();		/* 分叉进程 */
		if( pid == 0 )
		{		/* 子进程中 */
			close(l_s32ServerFd);		/* 在子进程中关闭服务器的侦听 */
			process_conn_server(l_s32ClientrFd);/* 处理连接 */
		}else
		{
			close(l_s32ClientrFd);		/* 在父进程中关闭客户端的连接 */
		}
	}
}
