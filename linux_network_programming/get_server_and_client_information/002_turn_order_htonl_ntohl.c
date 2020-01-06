/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: 002_turn_order_htonl_ntohl.c 
*BlogAddr: caibiao-lee.blog.csdn.net
*Description: 网络字节序与主机字节序的转换
*Date:     2020-01-04
*Author:   Caibiao Lee
*Version:  V1.0
*Others: 网络字节序是大端字节序
		 这里测试使用的PC机是小端字节序，同类型函数有

       uint32_t htonl(uint32_t hostlong);
       uint16_t htons(uint16_t hostshort);
       uint32_t ntohl(uint32_t netlong);
       uint16_t ntohs(uint16_t netshort);
*History:
***********************************************************/
#include <stdio.h>
#include <arpa/inet.h>

/* 联合类型的变量类型，用于测试字节序
*  	成员value的高低端字节可以由成员type按字节访问
 */
/* 16位 */
typedef union
{
	unsigned short int value;
	unsigned char byte[2];	
}to16;
/* 32位 */
typedef union
{
	unsigned long int value;
	unsigned char byte[4];	
}to32;

#define BITS16 16	/*16位*/
#define BITS32 32	/*32位*/
/* 按照字节打印，begin为字节开始，
*  flag为BITS16表示16位，
*  flag为BITS32表示32位， 
*/
void showvalue(unsigned char *begin, int flag)
{
	int num = 0, i = 0;
	if(flag == BITS16)
	{
		num = 2;	
	}else if(flag == BITS32)
	{
		num = 4;	
	}
	
	for(i = 0; i< num; i++)
	{
		printf("%x ",*(begin+i));	
	}
	printf("\n");
}

int main(int argc, char *argv)
{	
	to16 v16_orig, v16_turn1,v16_turn2;	/*一个to16类型变量*/
	to32 v32_orig, v32_turn1,v32_turn2; /*一个to32类型变量*/
	
	
	v16_orig.value = 0xabcd;		/* 赋值为0xabcd */
	v16_turn1.value = htons(v16_orig.value);/*第一次转换*/
	v16_turn2.value = ntohs(v16_turn1.value);/*第二次转换*/
	
	v32_orig.value = 0x12345678;	/* 赋值为0x12345678 */
	v32_turn1.value = htonl(v32_orig.value);/*第一次转换*/
	v32_turn2.value = ntohl(v32_turn1.value);/*第二次转换*/
	
	/* 打印结果 */
	printf("16 host to network byte order change:\n");	
	printf("\torig:\t");showvalue(v16_orig.byte, BITS16);	/* 16位数值的原始值 */
	printf("\t1 times:");showvalue(v16_turn1.byte, BITS16); /* 16位数值的第一次转换后的值 */
	printf("\t2 times:");showvalue(v16_turn2.byte, BITS16); /* 16位数值的第二次转换后的值 */
	
	printf("32 host to network byte order change:\n");
	printf("\torig:\t");showvalue(v32_orig.byte, BITS32);   /* 32位数值的原始值 */
	printf("\t1 times:");showvalue(v32_turn1.byte, BITS32); /* 32位数值的第一次转换后的值 */
	printf("\t2 times:");showvalue(v32_turn2.byte, BITS32); /* 32位数值的第二次转换后的值 */
	
	return 0;	
}
