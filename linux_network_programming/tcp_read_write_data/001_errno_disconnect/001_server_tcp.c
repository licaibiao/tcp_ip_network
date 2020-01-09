#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/in.h>
#include <signal.h>

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

/* 服务器对客户端的处理 */
void process_conn_server(int s32SocketFd)
{
	ssize_t size = 0;
	char buffer[1024];	/* 数据的缓冲区 */
	
	for(;;)
	{	/* 循环处理过程 */
		/* 从套接字中读取数据放到缓冲区buffer中 */
		size = read(s32SocketFd, buffer, 1024);	
		if(size == 0)
		{/* 没有数据 */
			return;	
		}
		
		/* 构建响应字符，为接收到客户端字节的数量 */
		sprintf(buffer, "%d bytes altogether\n", size);
		write(s32SocketFd, buffer, strlen(buffer)+1);/* 发给客户端 */
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
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);/* 本地地址 */
	server_addr.sin_port = htons(PORT);			/* 服务器端口 */
	
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
