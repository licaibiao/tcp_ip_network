/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: IPAddrInterFace.cpp
*Date:     2019-06-22
*Author:   Caibiao Lee
*Version:  V1.0
*Description:
*History:
***********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>


#include "IPAddrInterFace.h"

/************************************************* 
Function:    IPAddWriteToPipe  
Description: 将IP地址写入管道中。
Input: u32IPAddr
output:none
Return:0 成功，非0，失败
Others:注意命名管道的读写规则
	1.以非阻塞只写方式打开时，在多进程中同时写入数据，注意写入的原子性。
	2.以非阻塞只写方式打开时，如果没有一个进程在读管道，打开会失败。
	3.以非阻塞只写方式打开时，如果所有读管道进程关闭，写进程会收到SIGPIPE信号
	   如果写进程不对SIGPIPE信号进行处理，会导致写进程退出。
Author: Caibiao Lee
Date:   2019.06.21
*************************************************/
static int IPAddWriteToPipe(unsigned int u32IPAddr)
{
	int  l_s32Ret = 0;
    int  l_s32PipeFd = -1;
	int  l_s32SendCount = 3;
    char l_arrs8Buffer[32] = {0};
	IP_ADDR_NUM_S l_stIpAddr = {0};

	/**判断管道是否存在**/
    if(access(PIPE_NAME, F_OK) == -1)
    {
        printf ("Create the fifo pipe.\n");
        l_s32Ret = mkfifo(PIPE_NAME, 0777);
 
        if(l_s32Ret != 0)
        {
            fprintf(stderr, "Could not create fifo %s\n", PIPE_NAME);
           	return l_s32Ret;
        }
    }

	/**以非阻塞的方式去打开管道**/
    l_s32PipeFd = open(PIPE_NAME, O_NONBLOCK|O_WRONLY);
    if(l_s32PipeFd !=-1)
    {
    	l_stIpAddr.u8StartFlag  = IP_START_FLAG;
		l_stIpAddr.u8EndFlag    = IP_END_FLAG;
		l_stIpAddr.u32IPAddrNum = u32IPAddr;
		memcpy(l_arrs8Buffer,&l_stIpAddr,sizeof(IP_ADDR_NUM_S));
		
		l_s32Ret = write(l_s32PipeFd, l_arrs8Buffer, sizeof(IP_ADDR_NUM_S));
		if(l_s32Ret == -1)
		{
			while((l_s32SendCount--)>0)
			{
				sleep(1);
				if(-1 != write(l_s32PipeFd, l_arrs8Buffer, sizeof(IP_ADDR_NUM_S)))
				{
					l_s32Ret = 0;
					break;
				}else
				{
					l_s32Ret = -1;
				}
			}			
		}	
    }
	else
	{
		printf("open pipe errror !\n");
		l_s32Ret  = -1;
	}
	
	close(l_s32PipeFd);
	
	return l_s32Ret;
}


/************************************************* 
Function:    lcb_connect  
Description: 重新封装connet函数，与connect函数的应用
			 完全一致
Input: connect 系统函数的返回值
Return:
Others: 在这个函数中，将IP地址的十进制数值写入到管道中
	通过wsd_GetIpAddr接口获取IP值，以实现去耦合及进程间
	通行
Author: Caibiao Lee
Date:   2019.06.21
*************************************************/
int wsd_connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{
	int l_s32Ret = 0;
	unsigned int l_u32IpAddr = 0; 
	struct sockaddr_in *l_stAddrIn = NULL;
		
	l_s32Ret = connect(sockfd,addr,addrlen);

	l_stAddrIn  = (struct sockaddr_in *)addr;
	l_u32IpAddr = l_stAddrIn->sin_addr.s_addr;

	IPAddWriteToPipe(l_u32IpAddr);

	return l_s32Ret;
}


