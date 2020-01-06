#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT     12345
#define SERVER_IP_ADDR  "192.168.1.112" 
#define MAXDATASIZE     20480

int main(int argc, char *argv[])
{
	int l_s32Num;
	int l_s32Ret;
	int l_s32SuccesCount;
	int l_s32ErrorCount;
	int l_s32SendLen;
	int l_s32SendLostCount;
	int l_s32OtherCount;
	int l_s32SockFd;
	char l_arrs8Buf[MAXDATASIZE];
	struct hostent *l_stHostEnt;
	struct sockaddr_in l_stServerAddr;
	struct sockaddr_in l_stPeerAddr;
	socklen_t  l_stAddrLen;
	
	for(int i=0;i<MAXDATASIZE;i++)
	{
		l_arrs8Buf[i]=i;
	}
	
	if ((l_stHostEnt=gethostbyname(SERVER_IP_ADDR))==NULL)
	{
		printf("gethostbyname()error\n");
		exit(1);
	}

	if ((l_s32SockFd=socket(AF_INET, SOCK_DGRAM,0))==-1)
	{
		printf("socket() error\n");
		exit(1);
	}

	bzero(&l_stServerAddr,sizeof(l_stServerAddr));
	l_stServerAddr.sin_family = AF_INET;
	l_stServerAddr.sin_port = htons(SERVER_PORT);
	l_stServerAddr.sin_addr= *((struct in_addr *)l_stHostEnt->h_addr);
	
	l_s32Num = 0;
	l_s32SendLen = 0;
	l_s32OtherCount = 0;
	l_s32ErrorCount = 0;
	l_s32SuccesCount = 0;
	l_s32SendLostCount = 0;

	l_s32SendLen = 10000;
    while(l_s32Num<1000000)
	{
		l_s32Ret = sendto(l_s32SockFd,l_arrs8Buf,l_s32SendLen,0,(struct sockaddr *)&l_stServerAddr,sizeof(l_stServerAddr));
		if(-1==l_s32Ret)
		{
			l_s32ErrorCount++;
		}else if(l_s32Ret==l_s32SendLen)
		{
			l_s32SuccesCount++;
		}else if(l_s32Ret!=l_s32SendLen)
		{
			l_s32SendLostCount++;
			printf("l_s32Ret=%d l_s32SendLen=%d \n",l_s32Ret,l_s32SendLen);
		}else
		{
			l_s32OtherCount++;
			printf("l_s32Ret error :%d \n",l_s32Ret);
		}
		
		printf("send num:%d \n",l_s32Num++);
	}
	printf("each time Send Len %d \n",l_s32SendLen);
	printf("client send SuccesCount :%d \n",l_s32SuccesCount);
	printf("client send ErrorCount  :%d \n",l_s32ErrorCount);
	printf("client send LostCount   :%d \n",l_s32SendLostCount);
	printf("client send OtherCount  :%d \n",l_s32OtherCount);
	printf("client send TotalCount  :%d \n",l_s32Num);
	
	close(l_s32SockFd);
}
