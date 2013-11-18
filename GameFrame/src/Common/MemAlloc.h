#ifndef _XMEM_H_
#define _XMEM_H_

#include "Compact.h"
#include "Singleton.h"
#include "Mutex.h"

#define ALIGN_SIZE             8
#define MAX_MEMORY_PAGE        16384
#define MAX_CHUNK_NUM          2048

typedef struct MemList
{
    MemList* m_pNext;
}MemList;

typedef struct ChunkList
{
    ChunkList* m_pNext;
    void*      m_pData;
}ChunkList;

class MemAllocImpl
{
public:
    MemAllocImpl()
    {
        ::memset(m_pFreeList, 0, sizeof(m_pFreeList));
        ::memset(&m_ChunkList, 0, sizeof(m_ChunkList));
    }

    ~MemAllocImpl()
    {
        ChunkList* pChunkList = NULL;
        while(m_ChunkList.m_pNext != NULL)
        {
            pChunkList = m_ChunkList.m_pNext;
            m_ChunkList.m_pNext = pChunkList->m_pNext;
            free(pChunkList->m_pData);
        }
    }

    int ChunkIndex(int nSize)
    {
        int nIndex = (nSize - 1) / ALIGN_SIZE;
        assert(nIndex >= 0 && nIndex < MAX_CHUNK_NUM);
        return nIndex;
    }

    MemList* ChunkAlloc(int nIndex)
    {
        MemList* pNode = (MemList*)::malloc(MAX_MEMORY_PAGE);
        MemList* ret = pNode;

        int nSize = (nIndex + 1) * ALIGN_SIZE;
        for (int i = 0; i < MAX_MEMORY_PAGE / nSize - 1; ++ i)
        {
            pNode->m_pNext = pNode + nSize / sizeof(pNode);
            pNode = pNode->m_pNext;
        }

        pNode->m_pNext = NULL;

        return ret;
    }

    void* Alloc(int nSize)
    {
        int nIndex = ChunkIndex(nSize);
        assert(nIndex < MAX_CHUNK_NUM && nIndex >= 0);
        MemList* pNode = m_pFreeList[nIndex];
        if (pNode == NULL)
        {
            pNode = ChunkAlloc(nIndex);
            m_pFreeList[nIndex] = pNode->m_pNext;

            ChunkList* pChunkNode = (ChunkList*)Alloc(sizeof(ChunkList));
            pChunkNode->m_pNext = m_ChunkList.m_pNext;
            pChunkNode->m_pData = pNode;
            m_ChunkList.m_pNext = pChunkNode;
        }
        else { m_pFreeList[nIndex] = pNode->m_pNext; }

        return pNode;
    }

    void Dealloc(void* p, int nSize)
    {
        if (p != NULL)
        {
            int nIndex = ChunkIndex(nSize);
            assert(nIndex < MAX_CHUNK_NUM && nIndex >= 0);
            MemList* pNode = (MemList*)p;
            pNode->m_pNext = m_pFreeList[nIndex];
            m_pFreeList[nIndex] = pNode;
        }
    }

private:
    MemList*  m_pFreeList[MAX_CHUNK_NUM];
    ChunkList m_ChunkList;
};

class MemAlloc : public Singleton<MemAlloc>
{
public:
    MemAlloc()
        : m_pImpl(new MemAllocImpl())
    {}

    virtual ~MemAlloc() {}

    void* Alloc(int nSize) { return m_pImpl->Alloc(nSize); }
    void Dealloc(void* p, int nSize) { m_pImpl->Dealloc(p, nSize); }

private:
    MemAllocImpl* m_pImpl;
};

class MemAllocWithLock : public Singleton<MemAlloc>
{
public:
    MemAllocWithLock()
        : m_pImpl(new MemAllocImpl())
    {}

    virtual ~MemAllocWithLock() {}

    void* Alloc(int nSize) 
    {
        AutoLock lock(m_Mutex);
        return m_pImpl->Alloc(nSize); 
    }
    
    void Dealloc(void* p, int nSize) 
    { 
        AutoLock lock(m_Mutex);
        m_pImpl->Dealloc(p, nSize); 
    }

private:
    MemAllocImpl* m_pImpl;
    Mutex m_Mutex;
};

#define  MemAllocT MemAlloc::Instance()
#define  MemAllocWithLockT MemAllocWithLock::Instance()

#endif