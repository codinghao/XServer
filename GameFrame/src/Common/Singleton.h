#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template<class T>
class Singleton
{
protected:
    Singleton()  {}
    virtual ~Singleton() {}
public:
    static T& Instance()
    {
        if (m_Instance == NULL)
        {
            m_Instance = new (T)();
        }

        return *m_Instance;
    }

    static void Destory()
    {
        if (m_Instance != NULL)
        {
            delete m_Instance;
            m_Instance = NULL;
        }
    }

private:
    static T* m_Instance;
};

template<class T>
T* Singleton<T>::m_Instance = NULL;

#endif   //sigleton.h