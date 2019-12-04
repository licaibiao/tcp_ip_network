/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: ipc_msgstruct.h
*BlogAddr: https://blog.csdn.net/li_wen01
*Description:命令消息结构体定义和解析
*Date:	   2019-08-03
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#ifndef _IPC_MSGSTRUCT_H_
#define _IPC_MSGSTRUCT_H_

/**数据结构重定义**/
typedef unsigned char    byte;
typedef signed char	     INT8S;
typedef signed int	     INT32S;
typedef unsigned char    BYTE;
typedef unsigned short   WORD;
typedef unsigned int     DWORD;
typedef unsigned char    INT8U;
typedef unsigned short	 INT16U;
typedef unsigned int     INT32U;


#define IPCP_TRUE 			0
#define IPCP_FALSE			1

#define ARRAY_SIZE(_A) (sizeof(_A) / sizeof((_A)[0]))

/**通讯协议位定义**/
/************************************************************************************/
/** | 2Byet	| Byet	 | Byet	   | 2Byet  | 2*Byet |2Byet    |N*Byet | Byet | 2Byet  | **/
/** | 0xa5a5 | 源地址| 目标地址| 流水号 | 消息ID |消息长度 |消息体 | 校验 | 0x5a5a | **/
/** | 0  1   |   2   |    3    |  4  5  |  6  7  | 8  9    |10	   | 10+N | 10+N+1 | **/
/************************************************************************************/
#define MSG_SRC_ADDR 				(2)
#define MSG_TAR_ADDR 				(3)
#define MSG_SERIAL_NUM_ADDR 		(4)
#define MSG_CMID_ADDR 				(6)
#define MSG_MSG_LEN_ADDR 			(8)
#define MSG_ACK_CMID_ADDR 			(10)
#define MSG_ACK_SERIAL_NUM_ADDR 	(12)
#define MSG_ACK_RES_ADDR 			(14)
#define MSG_CONTENT_OFFSET 	 		(10)    /**消息内容开始位置**/

#define MSG_HEAD_LEN 				(10)
#define MSG_END_LEN  				(3)
#define MSG_ARCH_MSG_LEN  			(13)

#define QUEUE_MSG_HEAD              0xa5a5
#define QUEUE_MSG_END               0x5a5a


typedef struct MsgPack
{
    unsigned char SrcAddr;
    unsigned char TargAddr;
    unsigned short Len;
    unsigned char *Data;
    unsigned short CmdId;
}MSG_PACK_S;


#define  MSG_TEXT_SIZE       (2048)

typedef struct ARCH_MSG
{
    unsigned int MsgLen;           
    unsigned char SomeText[MSG_TEXT_SIZE];
}ARCH_MSG_S;


/*通用IPCP指令*/
#define QUEUE_DEBUG_CMD1       		 (0x0001)
#define QUEUE_DEBUG_CMD2       		 (0x0002)
#define QUEUE_DEBUG_CMD3       		 (0x0003)
#define QUEUE_DEBUG_CMD4       		 (0x0004)
#define QUEUE_DEBUG_CMD5       		 (0x0005)
#define QUEUE_DEBUG_CMD6       		 (0x0006)
#define QUEUE_DEBUG_CMD7       		 (0x0007)
#define QUEUE_DEBUG_CMD8       		 (0x0008)
#define QUEUE_DEBUG_CMD9       		 (0x0009)
#define QUEUE_DEBUG_CMDA       		 (0x000A)
#define QUEUE_DEBUG_MAX_CMD			 (0x000A)


/**消息定义**/
typedef struct
{
	DWORD   u32Alarm;      		   /* 报警 */
	DWORD 	u32Status;     		   /* 状态 */
	DWORD 	u32Latitude;   		   /* 纬度，百万分之一度 */  
	DWORD 	u32Longtitude; 		   /* 经度，百万分之一度 */  
	WORD	u16Altitude;		   /* 高程，米 */
	WORD	u16SpeedX10;   		   /* gps速度，1/10 km/h */
	WORD  	u16Direct;     		   /* 方向 */
 	BYTE    arrCardNo[16];	       /* 机动车牌号码			 */
	WORD    u16SensorSpeed;        /* 脉冲速度，1/10 km/h */	
	BYTE    u8CarCor;			   /* 车牌颜色 	 */	
}__attribute__ ((__packed__))MSG_0X0001_S;

typedef struct
{
	unsigned int  u32FileId;	    /**文件id，不为0时，按文件id查找**/ 
	unsigned char u8DeleteFlag;		/**删除标志:0：保留；1：删除**/ 
	unsigned char u8StopFlag;		/**停止上传标志:0：保留；1：停止上传	**/ 
	unsigned char u8srcPlat;	    /**下发命令的平台地址**/
}__attribute__ ((__packed__))MSG_0X0002_S;

typedef struct
{
	unsigned char u8Type;		     /**0人脸识别结果，1人头个数识别**/ 
	unsigned char u8Result;          /**type为0时，0成功，1失败**/ 
}__attribute__ ((__packed__))MSG_0X0003_S;

typedef struct
{
	unsigned char u8Interval;		/**时间间隔**/ 
	unsigned int  u32Duration;    	/**持续时间**/ 
}__attribute__ ((__packed__))MSG_0X0004_S;

typedef struct
{
    DWORD u32MultiId;				/**多媒体ID,小端内存**/
   	WORD  u16TotalNum;				/**下发拍照的总数**/
    WORD  u16CurNum;				/**当前图片序号**/ 	
	BYTE  u8PlatAddr;				/**平台地址**/
}__attribute__ ((__packed__))MSG_0X0005_S;

typedef struct
{
	DWORD   u32ModeEvent;      		/**休眠唤醒事件:1：休眠，2：唤醒*/	
}__attribute__ ((__packed__))MSG_0X0006_S;

typedef struct
{
	DWORD    u32ParaId;   		   /* 参数id: 0x1000010C*/
	BYTE     u8ParaLen;			   /* 参数长度: 4 */
	unsigned long long u64AlarmFlag;/* bit*/

}__attribute__ ((__packed__))MSG_0X0007_S;

typedef struct
{
	DWORD    u32ParaId;   		  /* 参数id: 0x1000010C*/
	BYTE     u8ParaLen;			  /* 参数长度: 4 */
	DWORD    u32TimeOut;		  /* 单位为秒*/
}__attribute__ ((__packed__))MSG_0X0008_S;

typedef struct
{
	DWORD    u32ParaId;   		  /* 参数id: 0x30000007*/
	BYTE     u8ParaLen;			  /* 参数长度: 4 */
	DWORD    u32Payload;		  /* 参数值:96:h264编码, 265:h265编码*/
}__attribute__ ((__packed__))MSG_0X0009_S;

typedef struct
{
	BYTE	u8ParaNum;            /* 参数总数 */
}__attribute__ ((__packed__))MSG_0X000A_S;


typedef int (*HandleHook_Func)(unsigned char*, unsigned short,unsigned char);

typedef struct 
{
    unsigned int u32MsgID;
    HandleHook_Func pFuncHandle;
}MSG_HANDLE_HOOK_S;


int IPCPMsg_Debug_Cmd1(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_Cmd2(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_Cmd3(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_Cmd4(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_Cmd5(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_Cmd6(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_Cmd7(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_Cmd8(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_Cmd9(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);
int IPCPMsg_Debug_CmdA(unsigned char *pu8SomeText, unsigned short u16Len, unsigned char u8SrcAddr);

#endif

