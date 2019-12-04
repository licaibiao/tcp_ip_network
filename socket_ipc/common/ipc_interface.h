/************************************************************
*Copyright (C),lcb0281at163.com lcb0281atgmail.com
*FileName: ipc_interface.c
*BlogAddr: https://blog.csdn.net/li_wen01
*Description:客户端网络连接，数据收发，消息解析函数接口定义实现
*Date:	   2019-08-03
*Author:   Caibiao Lee
*Version:  V1.0
*Others:
*History:
***********************************************************/
#ifndef _IPC_INTERFACE_H_
#define _IPC_INTERFACE_H_

#include "ipc_common.h"
#include "ipc_msgstruct.h"


/**获取流水号**/
int IPCP_Arch_Msg_AnalyzeGetFlowNum();

/**获取应答流水号**/
int IPCP_Arch_Msg_AnalyzeGetACKResult(ARCH_MSG_S *pstMsg);

/**获取流水号地址**/
int IPCP_Arch_Msg_AnalyzeGetRecFlow(ARCH_MSG_S *pstMsg);

/**获取源地址**/
int IPCP_Arch_Msg_ChangeSrcAddr(ARCH_MSG_S *pstMsg,unsigned  char SrcAddr);

/**获取目标地址**/
int IPCP_Arch_Msg_ChangeTargAddr(ARCH_MSG_S *pstMsg,unsigned  char TargAddr);

/**获取源地址**/
int IPCP_Arch_Msg_AnalyzeGetSrcAddr(ARCH_MSG_S *pstMsg);

/**获取目标地址**/
int IPCP_Arch_Msg_AnalyzeGetTargAddr(ARCH_MSG_S *pstMsg);

/**获取消息ID**/
int IPCP_Arch_Msg_AnalyzeGetCmdID(ARCH_MSG_S *pstMsg);

/**获取内容长度**/
int IPCP_Arch_Msg_AnalyzeGetLen(ARCH_MSG_S *pstMsg);

/**获取消息内容开始位置**/
void IPCP_Arch_Msg_PlatformStartP(unsigned char ** p, unsigned char* Data);

/**发送数据**/
int IPCP_Arch_Msg_PackSend(int s32ModuleAddr, MSG_PACK_S *pstMsg);

/**读取数据**/
int  IPCP_Arch_Msg_Recv(int s32ModuleAddr,ARCH_MSG_S *pstMsg);


#endif

