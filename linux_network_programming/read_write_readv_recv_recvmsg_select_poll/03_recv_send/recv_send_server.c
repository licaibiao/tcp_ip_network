/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: recv_send_server.c
*BlogAddr: caibiao-lee.blog.csdn.net
*Description: 
*Date:     2020-01-04
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
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

#define PORT 8888		/* 侦听端口地址 */
#define BACKLOG 2		/* 侦听队列长度 */

static struct iovec *vs=NULL,*vc=NULL;
/* 服务器对客户端的处理 */

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

/* 服务器对客户端的处理 */
void process_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[1024];	/* 数据的缓冲区 */
	
	for(;;){/* 循环处理过程 */
		/* 从套接字中读取数据放到缓冲区buffer中 */
		size = recv(s, buffer, 1024,0);	
		if(size == 0){/* 没有数据 */
			return;	
		}
		
		/* 构建响应字符，为接收到客户端字节的数量 */
		sprintf(buffer, "%d bytes altogether\n", (int)size);
		send(s, buffer, strlen(buffer)+1,0);/* 发给客户端 */
	}	
}


int main(int argc, char *argv[])
{
	int ss,sc;		/* ss为服务器的socket描述符,sc为客户端的socket描述符 */
	struct sockaddr_in server_addr; /* 服务器地址结构 */
	struct sockaddr_in client_addr;	/* 客户端地址结构 */
	int err;	/* 返回值 */
	pid_t pid;	/* 分叉的进行id */
	
	signal(SIGINT, sig_proccess);
	signal(SIGPIPE, sig_proccess);
	
	
	/* 建立一个流式套接字 */
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0){/* 出错 */
		printf("socket error\n");
		return -1;	
	}	
	
	/* 设置服务器地址 */
	bzero(&server_addr, sizeof(server_addr));	/* 清0 */
	server_addr.sin_family = AF_INET;			/* 协议族 */
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);/* 本地地址 */
	server_addr.sin_port = htons(PORT);			/* 服务器端口 */
	
	/* 绑定地址结构到套接字描述符 */
	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err < 0){/* 出错 */
		printf("bind error\n");
		return -1;	
	}
	
	/* 设置侦听 */
	err = listen(ss, BACKLOG);
	if(err < 0){/* 出错 */
		printf("listen error\n");
		return -1;	
	}
	
	/* 主循环过程 */
	for(;;)	{
		int addrlen = sizeof(struct sockaddr);
		/* 接收客户端连接 */
		sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
		if(sc < 0){		/* 出错 */
			continue;	/* 结束本次循环 */
		}	
		
		/* 建立一个新的进程处理到来的连接 */
		pid = fork();		/* 分叉进程 */
		if( pid == 0 ){		/* 子进程中 */
			close(ss);		/* 在子进程中关闭服务器的侦听 */
			process_conn_server(sc);/* 处理连接 */
		}else{
			close(sc);		/* 在父进程中关闭客户端的连接 */
		}
	}
}



