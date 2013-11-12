#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include "../Delegate.h"
#include "Buffer.h"

class Socket;
class Operation;

typedef Delegate<void (Socket* /*_socket*/, Buffer* /*_buffer*/, int /*_errorCode*/)> ReadHandler;
typedef Delegate<void (Socket* /*_socket*/, Buffer* /*_buffer*/, int /*_errorCode*/)> WriteHandler;
typedef Delegate<void (Socket* /*_socket*/, Buffer* /*_buffer*/, int /*_errorCode*/)> AcceptHandler;
typedef Delegate<void (Operation* /*_operation*/, int /*_errorCode*/)> DestoryHandler;

#endif