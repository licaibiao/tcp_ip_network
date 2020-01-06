/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: 005_check_is_socket_type.c
*BlogAddr: caibiao-lee.blog.csdn.net
*Description: 判断文件描述符是不是socket类型
*Date:     2020-01-04
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>


int issockettype(int fd)
{
	struct stat st;
	int err =	fstat(fd, &st);
	
	if( err < 0 )	{
		return -1;		
	}
	
	if((st.st_mode & S_IFMT) == S_IFSOCK)	{
		return 1;
	}	else{
		return 0;
	}
}

int main(void)
{
	int ret = issockettype(0);
	printf("value %d\n",ret);
	
	int s = socket(AF_INET, SOCK_STREAM,0);
	ret = issockettype(s);
	printf("value %d\n",ret);
	
	
	return 0;	
}
