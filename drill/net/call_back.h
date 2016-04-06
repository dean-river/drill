#ifndef DRILL_NET_CALLBACK_H_
#define DRILL_NET_CALLBACK_H_
#include <drill/common/buffer.h>
#include <drill/common/time.h>

#include <functional>
#include <memory>

using drill::common::Buffer;
using drill::common::Time;

namespace drill {
namespace net{

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void (const TcpConnectionPtr&, size_t)> HighWaterMarkCallback;

// the data has been read to (buf, len)
typedef std::function<void (const TcpConnectionPtr&,
                              Buffer*,
                              Time)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Time receiveTime);

}
}

#endif
