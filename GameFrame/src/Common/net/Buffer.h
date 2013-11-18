#ifndef _BUFFER_H_
#define _BUFFER_H_

#define BUFFER_SIZE 4096

typedef struct _Buffer {
    _Buffer(char* _buf, int _maxLen, int _transferLen = 0)
        : m_Buffer(_buf)
        , m_MaxLen(_maxLen)
        , m_TransferLen(_transferLen)
    {}

    char* m_Buffer;
    int   m_MaxLen;
    int   m_TransferLen;
}Buffer;

#endif

