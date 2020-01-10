/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: 01_client_tcp.c
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
#define PORT 8888	/* 侦听端口地址 */

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


void process_conn_client(int s32SocketFd)
{
	int size = 0;
	char buffer[1024] = {0};
	char *sendData = "I am client";
	
	for(;;)
	{
		#if 1
		size = write(s32SocketFd, sendData, strlen(sendData)+1);	
		if(size!=strlen(sendData)+1)
		{
			printf("write data error size=%d errno=%d \n",size,errno);
			//return ;
		}
		
		size = read(s32SocketFd, buffer, 1024);
		if(size<=0)
		{
			printf("read data error size=%d errno=%d \n",size,errno);
			//return ;				
		}else
		{
			printf("recv Data: %s\n",buffer);
		}
		#endif
		sleep(1);

	}	
}

int main(int argc, char *argv[])
{

	struct sockaddr_in server_addr;	
	int l_s32SocketFd = 0;

		
	signal(SIGINT, sig_proccess);
	signal(SIGPIPE, sig_pipe);
	
	/* 建立一个流式套接字 */
	l_s32SocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(l_s32SocketFd < 0)
	{/* 出错 */
		printf("socket error\n");
		return -1;	
	}	
	
	/* 设置服务器地址 */
	bzero(&server_addr, sizeof(server_addr));		/* 清0 */
	server_addr.sin_family = AF_INET;				/* 协议族 */
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);/*服务器IP地址*//* 本地地址 */
	server_addr.sin_port = htons(PORT);				/* 服务器端口 */
	
	/* 连接服务器 */
	connect(l_s32SocketFd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	process_conn_client(l_s32SocketFd);	/* 客户端处理过程 */
	
	close(l_s32SocketFd);	/* 关闭连接 */
	
	return 0;
}

