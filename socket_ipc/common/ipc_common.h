/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: ipc_common.h
*BlogAddr: https://blog.csdn.net/li_wen01
*Description: 
	socket进程间通信数据结构及参数定义
	包括跨设备间TCP进程间通信和本地Unix Domain 
	Socket进程间通信
*Date:	   2019-08-03
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#ifndef _IPC_COMMON_H_
#define _IPC_COMMON_H_

/**跨设备TCP socket进程间通信**/
#define MAX_EXTARN_DEV_NUM			  2      /**最大连接的网络设备数**/
#define EACH_TCP_DEV_MAX_CLIENT_NUM   10     /**每个外接网络设备客户端数**/
#define TCP_SERVER_PORT				  6666   /**TCP连接服务端IP端口**/

/**TCP连接最大支持客户端数**/
#define TCP_SERVER_LISTEN_MAX_NUM	 ((MAX_EXTARN_DEV_NUM)*(EACH_TCP_DEV_MAX_CLIENT_NUM))

#define TCP_SERVER_IP				"192.168.1.111"
#define TCP_CLIENT_DEVICE1_IP		"192.168.1.111"  /**网络设备1 IP地址**/
#define TCP_CLIENT_DEVICE2_IP		"192.168.1.112"  /**网络设备2 IP地址**/
#define TCP_CLIENT_DEVICE1_NO		1
#define TCP_CLIENT_DEVICE2_NO		2

/**网络设备各客户端TCP端口**/
#define TCP_DEVICE_CLIENT0_POART	9000
#define TCP_DEVICE_CLIENT1_POART	9001
#define TCP_DEVICE_CLIENT2_POART	9002
#define TCP_DEVICE_CLIENT3_POART	9003
#define TCP_DEVICE_CLIENT4_POART	9004
#define TCP_DEVICE_CLIENT5_POART	9005
#define TCP_DEVICE_CLIENT6_POART	9006
#define TCP_DEVICE_CLIENT7_POART	9007
#define TCP_DEVICE_CLIENT8_POART	9008
#define TCP_DEVICE_CLIENT9_POART	9009
#define TCP_DEVICE_CLIENT_MAX_POART 9009


/**本地Unix Domain Socket进程间通信**/
#define MAX_UDS_CLIENT_NUM			  20        /**最大socket域客户端数，对应最大本地通讯进程数**/
#define	SERVER_PATH		"../tmp/server_socket"   /**socket域服务端文件**/
#define CLIENT_PACHT    "../tmp/client_socket"   /**socket域客户端文件前缀**/

/**本地客户端模块序号定义**/
#define CLIENT_MIN_ADDR					1
#define LOCAL_CLIENT_1_ADDR        		1
#define LOCAL_CLIENT_2_ADDR        		2
#define LOCAL_CLIENT_3_ADDR        		3
#define LOCAL_CLIENT_4_ADDR        		4
#define LOCAL_CLIENT_5_ADDR        		5
#define LOCAL_CLIENT_6_ADDR        		6
#define LOCAL_CLIENT_7_ADDR        		7
#define LOCAL_CLIENT_8_ADDR        		8
#define LOCAL_CLIENT_9_ADDR        		9
#define LOCAL_CLIENT_10_ADDR        	10
#define LOCAL_CLIENT_11_ADDR        	11
#define LOCAL_CLIENT_12_ADDR        	12
#define LOCAL_CLIENT_13_ADDR        	13
#define LOCAL_CLIENT_14_ADDR        	14
#define LOCAL_CLIENT_15_ADDR        	15
#define LOCAL_CLIENT_16_ADDR        	16
#define LOCAL_CLIENT_17_ADDR        	17
#define LOCAL_CLIENT_18_ADDR        	18
#define LOCAL_CLIENT_19_ADDR        	19
#define LOCAL_CLIENT_MAX_ADDR        	19


/**网络设备1客户端模块序号定义**/
#define DEV_CLIENT_MIN_ADDR        		20
#define DEV1_CLIENT_0_ADDR        		20
#define DEV1_CLIENT_1_ADDR        		21
#define DEV1_CLIENT_2_ADDR        		22
#define DEV1_CLIENT_3_ADDR        		23
#define DEV1_CLIENT_4_ADDR        		24
#define DEV1_CLIENT_5_ADDR        		25
#define DEV1_CLIENT_6_ADDR        		26
#define DEV1_CLIENT_7_ADDR        		27
#define DEV1_CLIENT_8_ADDR        		28
#define DEV1_CLIENT_9_ADDR        		29

/**网络设备1客户端模块序号定义**/
#define DEV2_CLIENT_0_ADDR        		30
#define DEV2_CLIENT_1_ADDR        		31
#define DEV2_CLIENT_2_ADDR        		32
#define DEV2_CLIENT_3_ADDR        		33
#define DEV2_CLIENT_4_ADDR        		34
#define DEV2_CLIENT_5_ADDR        		35
#define DEV2_CLIENT_6_ADDR        		36
#define DEV2_CLIENT_7_ADDR        		37
#define DEV2_CLIENT_8_ADDR        		38
#define DEV2_CLIENT_9_ADDR        		39
#define DEV_CLIENT_MAX_ADDR        		39
#define CLIENT_MAX_ADDR      			39

extern const char * gc_au8DeviceNoMap[MAX_EXTARN_DEV_NUM]; 
extern const unsigned int gc_as32DeviceModuleMap[TCP_SERVER_LISTEN_MAX_NUM][3];


/**获取随机数**/
int IPCP_GetRandomReal(int low, int up) ;

/**打印消息**/
int IPCP_PrintHexBytes(unsigned char *pu8Bin,int s32Len);


#endif
