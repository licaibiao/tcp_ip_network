/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: recv_send_client.c
*BlogAddr: caibiao-lee.blog.csdn.net
*Description: 
*Date:     2020-01-04
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define PORT 8888	/* 侦听端口地址 */
static struct iovec *vs=NULL,*vc=NULL;
static int s;
void sig_proccess(int signo)
{
	printf("Catch a exit signal\n");
	/* 释放资源 */	
	if(NULL!=vc)
	{
		free(vc);
	}
	
	if(NULL!=vs)
	{
		free(vs);
	}
	
	_exit(0);	
}

void sig_pipe(int sign)
{
	printf("Catch a SIGPIPE signal\n");
	
	/* 释放资源 */	
	if(NULL!=vc)
	{
		free(vc);
	}
	
	if(NULL!=vs)
	{
		free(vs);
	}
	_exit(0);
}

void sig_proccess_client(int signo)
{
	printf("Catch a exit signal\n");
	if(s>0)
	{
		close(s);
		s=-1;
	}
	exit(0);	
}



/* 客户端的处理过程 */
void process_conn_client(int s)
{
	ssize_t size = 0;
	char buffer[1024];	/* 数据的缓冲区 */
	
	for(;;){/* 循环处理过程 */
		/* 从标准输入中读取数据放到缓冲区buffer中 */
		size = read(0, buffer, 1024);
		if(size > 0){/* 读到数据 */
			send(s, buffer, size,0);		/* 发送给服务器 */
			size = recv(s, buffer, 1024,0);/* 从服务器读取数据 */
			write(1, buffer, size);		/* 写到标准输出 */
		}
	}	
}

int main(int argc, char *argv[])
{

	struct sockaddr_in server_addr;	/* 服务器地址结构 */
	int err;/* 返回值 */
		
	if(argc == 1){
		printf("PLS input server addr\n");
		return 0;	
	}
	signal(SIGINT, sig_proccess);
	signal(SIGPIPE, sig_pipe);
	
	/* 建立一个流式套接字 */
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0){/* 出错 */
		printf("socket error\n");
		return -1;	
	}	
	
	/* 设置服务器地址 */
	bzero(&server_addr, sizeof(server_addr));		/* 清0 */
	server_addr.sin_family = AF_INET;				/* 协议族 */
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);/* 本地地址 */
	server_addr.sin_port = htons(PORT);				/* 服务器端口 */
	
	/* 将用户输入的字符串类型的IP地址转为整型 */
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);	
	/* 连接服务器 */
	connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
	process_conn_client(s);	/* 客户端处理过程 */
	close(s);	/* 关闭连接 */
}

