#ifndef _MESSAGE_H_
#define _MESSAGE_H_

typedef struct Header
{    
    ushort m_nNo;  //Э���
    ushort m_nLen; //������
}Header;

typedef struct Packet
{
    Header m_Header;
    char* m_pData;
};

#define PACKET_HEADER_SIZE sizeof(Header)
#define PACKET_SIZE(pkt) (PACKET_HEADER_SIZE + pkt.m_Header.nLen)

#endif