#ifndef _BUFFER_H_
#define _BUFFER_H_

typedef struct _Buffer {
    _Buffer(char* _buf, int _len)
        : m_Buffer(_buf)
        , m_Len(_len)
    {}

    char* m_Buffer;
    int   m_Len;
}Buffer;

#endif

