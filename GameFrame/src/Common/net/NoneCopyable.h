#ifndef _NONE_COPYABLE_H_
#define _NONE_COPYABLE_H_

class NoneCopyable
{
protected:
    NoneCopyable() {}
    ~NoneCopyable() {}
private:
    NoneCopyable( const NoneCopyable& );
    const NoneCopyable& operator=( const NoneCopyable& );
};

#endif