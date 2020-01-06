/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: 007_get_peer_name.c
*BlogAddr: caibiao-lee.blog.csdn.net
*Description: 获取与某个套接字关联的外地协议地址
*Date:     2020-01-04
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
	getsockname函数用于获取与某个套接字关联的本地协议地址
	getpeername函数用于获取与某个套接字关联的外地协议地址
*History:
***********************************************************/
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

int main(void)
{
	struct sockaddr_in addr;
	socklen_t len;
	int s = socket(AF_INET, SOCK_STREAM,0);
	
	getpeername(s, (struct sockaddr*)&addr, &len);
	printf("peer ");
	
	
	return 0;	
}