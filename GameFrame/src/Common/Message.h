#ifndef _MESSAGE_H_
#define _MESSAGE_H_

typedef struct _Header
{    
    ushort m_nNo;  //协议号
    ushort m_nLen; //包长度
}Header;

typedef struct _Packet
{
    Header m_Header;
    char* m_pData;
}Packet;

#define PACKET_HEADER_SIZE sizeof(Header)
#define PACKET_SIZE(pkt) (PACKET_HEADER_SIZE + pkt.m_Header.m_nLen)

#endif