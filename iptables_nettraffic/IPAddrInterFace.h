/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: IPAddrInterFace.cpp
*Date:     2019-06-22
*Author:   Caibiao Lee
*Version:  V1.0
*Description: IP地址设置和获取模块
*Others: 
*History:
***********************************************************/

#ifndef __INTER_FACE_H_
#define __INTER_FACE_H_

#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_START_FLAG	0xa5
#define IP_END_FLAG		0x5a
#define PIPE_NAME		"/tmp/ip_addr"

/**ip地址的十进制数值**/
typedef struct 
{
	unsigned char u8StartFlag;
	unsigned int  u32IPAddrNum;
	unsigned char u8EndFlag;
}IP_ADDR_NUM_S; 

int wsd_connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);


#endif

