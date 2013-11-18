#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#define AtomicCountInc(count) ::InterlockedIncrement(&count)
#define AtomicCountDec(count) ::InterlockedDecrement(&count)

#endif