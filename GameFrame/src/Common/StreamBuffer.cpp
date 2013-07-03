#include "StreamBuffer.h"

StreamBuffer::StreamBuffer()
    , m_nReadLen(0)
{
    ::memset(&m_DataBuffer, 0, sizeof(DataBuffer));
}

StreamBuffer::~StreamBuffer()
{

}

void StreamBuffer::AllocPacket(Packet* pData)
{
    assert(pData != NULL);

    pData->m_pData = MemAllocT.Alloc(pData->m_Header.m_nLen);
}

void StreamBuffer::DeallocPacket(Packet* pData)
{
    assert(pData != NULL);

    MemAllocT.Dealloc(pData->m_pData, pData->m_Header.m_nLen);
}

Packet StreamBuffer::Read()
{
    if (m_BufferQueue.empty())
        return ;

    Packet pkt = m_BufferQueue.front();
    m_BufferQueue.pop_front();

    return pkt;
}

void StreamBuffer::Write(Packet* pData)
{
    assert(pData != NULL);

    Packet pkt = *pData;
    AllocPacket(&pkt);
    ::memcpy(pData, pData->m_pData, pkt.m_Header.m_nLen);

    m_BufferQueue.push_back(pkt);
}

void StreamBuffer::Read(char* pData, int& nLen)
{
    assert(pData != NULL);

    Packet& pkt = m_BufferQueue.front();
    ::memcpy(pData, &pkt.m_Header, PACKET_HEADER_SIZE);
    ::memcpy(pData + PACKET_HEADER_SIZE, pkt.m_pData, pkt.m_Header.m_nLen);
    nLen = PACKET_SIZE(pkt);

    DeallocPacket(pkt);
    m_BufferQueue.pop_front();
}

void StreamBuffer::Write(char* pData, int nLen)
{
    while (nLen <= 0)
    {
        int nLeftLen = PACKET_HEADER_SIZE - m_nReadLen;
        if (m_DataBuffer.m_pData == NULL && nLeftLen > 0)
        {
            if (nLen >= nLeftLen)
            {
                ::memcpy(&m_DataBuffer.m_Header + m_nReadLen, pData, nLeftLen);
                AllocPacket(&m_DataBuffer);
            }
            else
            {
                ::memcpy(&m_DataBuffer.m_Header + m_nReadLen, pData, nLen);
            }

            nLen -= nLeftLen;
            m_nReadLen += nLeftLen;
            pData = pData + nLeftLen;
        }
        else if (m_DataBuffer.m_pData != NULL && nLeftLen <= 0)
        {
            int nMinLen = Min(m_DataBuffer.m_Header.m_nLen, nLen);
            ::memcpy(m_DataBuffer.m_pData, pData, nMinLen);

            nLen -= nMinLen;
            m_nReadLen += nMinLen;
            pData = pData + nMinLen;

            if (m_nReadLen == m_DataBuffer.m_Header.m_nLen)
            {
                m_BufferQueue.push_back(m_DataBuffer);
                ::memset(&m_DataBuffer, 0, sizeof(Packet));
                m_nReadLen = 0;
            }
        }
        else
        {
            break;
        }
    }
}