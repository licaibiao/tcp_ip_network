/************************************************************
*Copyright (C), 2017-2027,lcb0281at163.com lcb0281atgmail.com
*FileName: NetTrafficStati.h
*Date:     2019-06-22
*Author:   Caibiao Lee
*Version:  V1.0
*Description:网络流量统计
*Others:
*History:
***********************************************************/
#ifndef __NET_TRAFFIC_STATI_H_
#define __NET_TRAFFIC_STATI_H_

/**链路层数据收发获取地址**/
#define NETWORK_CARD_TX "/sys/class/net/ens33/statistics/tx_bytes"   
#define NETWORK_CARD_RX "/sys/class/net/ens33/statistics/rx_bytes"  

/**流量数据保存地址**/
#define NET_FLOW_REC_PATH "/mnt/NetTrafficStatistics.rec" 

/**单次开机最大支持记录IP数量**/
#define MAX_IP_COUNT 			64  


/**用来设置Iptable的IP列表**/
typedef struct
{
	unsigned int  u32IPAddrNum[MAX_IP_COUNT];
	unsigned char u8UsedIpCount;
	unsigned char u8InitFlag;
}IP_TABLE_S;

/**流量数据**/
typedef struct 
{
	char s8Date[3];
	unsigned long long u64NetWorkCount; /**网卡流量**/			
	unsigned long long u64IptableCount; /**监控流量**/				
}NetFlowCount_S;


class NetTrafficStati
{
public:
	NetTrafficStati();
	~NetTrafficStati();
	static NetTrafficStati *NTS_Instance();
private :
	int IpTable_Init(IP_TABLE_S *stIpTable);
	int IpTable_Check(IP_TABLE_S *stIpTable,unsigned int IpNum);
	int IpTable_Add(IP_TABLE_S *stIpTable,unsigned int IpNum);
	int iptables_rulesAdd(struct sockaddr_in *addr);

	int NTS_GetNetWorkCardFlow(unsigned long long *u64Count);
	int NTS_ReadIptableIOByte(unsigned long long *u64Count);

	int NTS_GetIpFromPiPe(void);
	
public:		
	int NTS_AddIpToIPTable(void);
	int NTS_UpdateFlowData(void);

private :
	int m_s32PipeFileID;
	IP_TABLE_S  m_stIpTable;
	IP_TABLE_S* m_pstIpTable;
	NetFlowCount_S m_stFlowCount;

};

#endif



