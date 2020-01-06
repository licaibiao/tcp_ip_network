#include <sys/types.h>
#include <sys/socket.h>	/*socket()/bind()*/
#include <linux/in.h>	/*struct sockaddr_in*/
#include <string.h>		/*memset()*/
#include <stdio.h>

#define PORT_SERV 8888
#define BUFF_LEN  256

static void udpclie_echo(int s, struct sockaddr *to)
{
	char buff[BUFF_LEN] = "UDP TEST";
	struct sockaddr_in from;
	int len = sizeof(*to);
	
	while(1)
	{
			sendto(s, buff, BUFF_LEN, 0, to, BUFF_LEN);	
			sleep(1);
	}
	

#if 0	
	sendto(s, buff, BUFF_LEN, 0, to, len);
	int i = 0;
	for(i = 0; i< 4; i++)
	{
		memset(buff, 0, BUFF_LEN);
		int err = recvfrom(s, buff, 1, 0, (struct sockaddr*)&from, &len);
		printf("%dst:%c,err:%d\n",i,buff[0],err);
	}
	printf("recved:%s\n",buff);
#endif	
}


int main(int argc, char *argv[])
{
	int l_s32Len;
	int l_s32SocketId;
	struct sockaddr_in l_stAddrServer; /*服务器地址*/
	struct sockaddr_in l_stAddrLocal;  /*本地地址*/
	
	l_s32Len = sizeof(l_stAddrLocal);
	
	/*填充服务器地址*/
	l_s32SocketId = socket(AF_INET, SOCK_DGRAM, 0);/*建立数据报套接字*/
	memset(&l_stAddrServer, 0, sizeof(l_stAddrServer));
	
	l_stAddrServer.sin_family = AF_INET;     /*AF_INET协议族*/
	l_stAddrServer.sin_addr.s_addr = inet_addr("192.168.1.112");/*地址为127.0.0.1*/
	l_stAddrServer.sin_port = htons(PORT_SERV);
	
	//*连接服务器*/
	//connect(l_s32SocketId, (struct sockaddr*)&l_stAddrServer, sizeof(l_stAddrServer));
	//sendto(fd, buf, strlen(buf), 0,				(struct sockaddr *)&srvaddr, len);	
	udpclie_echo(l_s32SocketId, (struct sockaddr*)&l_stAddrServer);
	
	close(l_s32SocketId);
	return 0;	
}

