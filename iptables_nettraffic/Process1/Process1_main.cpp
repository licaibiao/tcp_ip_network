/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: Process1_main.cpp
*Date:     2019-06-22
*Author:   Caibiao Lee
*Version:  V1.0
*Description:调用wsd_connect接口将IP地址传递给统计进程
*Others: 
***********************************************************/
#include<stdio.h>       
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<termios.h>
#include<sys/types.h>   
#include<sys/stat.h>    
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<signal.h>

#include "IPAddrInterFace.h"


#define MAXLINE 256
#define PORT    6666
int fd;
/*
linux ctrl + C 会产生 SIGINT信号
接收到SIGINT 信号进入该函数
*/
void stop(int signo)
{
    printf("client stop\n");
	close(fd);  
	_exit(0);
}
 
/*客户端处理函数*/
void client_process(void)
{
	char readbuff[MAXLINE];
	char writebuff[MAXLINE];
	char * write = "I am client";
	int num = 0;
	
	while(1)
	{
		num = recv(fd,readbuff,MAXLINE,0);/*接收服务端的数据，recv在这里如果没有数据会阻塞*/
		if(num > 0)
		{
			printf("client read data : %s \n",readbuff);
			send(fd, write, strlen(write)+1, 0); /*接收到数据后再向服务端发送一个字符串*/
		}
		else if(num == 0)/*recv返回值为0 的时候表示服务端已经断开了连接*/
		{
			stop(1); /*执行退出操作*/
		}
	}
}
 
int main(int argc, char** argv)
{
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int ret;
 
	fd = socket(AF_INET,SOCK_STREAM,0);/*建立流式套接字*/
	if(fd < 0)
	{
		printf("clinet socket err \n");
	}
 
	/*设置服务端地址*/
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET; 				/*AF_INET表示 IPv4 Intern 协议*/
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);/*INADDR_ANY 可以监听任意IP */
	server_addr.sin_port = htons(PORT);				/*设置端口*/
 
	inet_pton(AF_INET,"192.168.20.221",&server_addr.sin_addr);/*将用户输入的字符串类型的IP地址转为整型*/
	//connect(fd,(struct sockaddr*)&server_addr,sizeof(server_addr));/*连接服务器*/
	wsd_connect(fd,(struct sockaddr*)&server_addr,sizeof(server_addr));/*连接服务器*/
	signal(SIGINT,stop);	/*注册SIGINT信号*/
	client_process();		/*进入处理函数*/
	
	close(fd);/*关闭文件*/
	return 0;
}
 
 
 




