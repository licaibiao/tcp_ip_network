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
#define MAXDATASIZE     100

#define SEND_STRING		"HELLO WORLD"


int main(int argc, char *argv[])
{
	int l_s32Num;
	int l_s32SockFd;
	char l_arrs8Buf[MAXDATASIZE];
	struct hostent *l_stHostEnt;
	struct sockaddr_in l_stServerAddr;
	struct sockaddr_in l_stPeerAddr;
	socklen_t  l_stAddrLen;
	
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
	
    while(1)
	{
		/**send data**/
		sendto(l_s32SockFd,SEND_STRING,strlen(SEND_STRING),0,(struct sockaddr *)&l_stServerAddr,sizeof(l_stServerAddr));
		
		/**recv data**/
		l_stAddrLen=sizeof(l_stServerAddr);
		l_s32Num=recvfrom(l_s32SockFd,l_arrs8Buf,MAXDATASIZE,0,(struct sockaddr *)&l_stPeerAddr,&l_stAddrLen);
		if((l_s32Num)==-1)
		{
			printf("recvfrom() error\n");
			exit(1);
		}else
		{
			printf("recv: %s \n",l_arrs8Buf);
			
		}
		
		if (l_stAddrLen != sizeof(l_stServerAddr) ||memcmp((const void *)&l_stServerAddr, (const void *)&l_stPeerAddr,l_stAddrLen) != 0)
		{
			printf("Receive message from otherserver.\n");
			continue;
		}
		
		sleep(1);
	}		
	close(l_s32SockFd);
}
