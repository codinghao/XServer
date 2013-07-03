#ifndef _STREAM_BUFFER_H_
#define _STREAM_BUFFER_H_

#include "Compact.h"
#include "Message.h"
#include <deque>

#define MAX_MSG_LENGTH 4096

class StreamBuffer
{
public:
    typedef std::deque<Packet> BufferQueueT;

    StreamBuffer();
    ~StreamBuffer();

    void AllocPacket(Packet* pData);
    void DeallocPacket(Packet* pData);

    Packet* Read();
    void Read(char* pData, int& nLen);
    void Write(char* pData, int nLen);
    void Write(Packet* pData);

private:
    int m_nReadLen;
    Packet m_DataBuffer;
    BufferQueueT m_BufferQueue;
};

#endif