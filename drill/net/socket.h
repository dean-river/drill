#ifndef DRILL_NET_SOCKET_H_
#define DRILL_NET_SOCKET_H_
#include <drill/net/addr_inet.h>
#include <string>

struct tcp_info;
namespace drill {
namespace net {

class Socket {

public:

     Socket(bool own = false);

    ~Socket();
    bool connect( AddrInet& addr) ;
    bool bind( AddrInet& addr) ;
    bool listen(int backlog = 5);

    int accept(AddrInet& peeraddr);

    void shutdown();
    void close();

    void setFd(int socketHandle);

    int getFd();

    int write(const void *data, int len);

    int read(void *data, int len);

    bool setKeepAlive(bool on) {
        return setIntOption(SO_KEEPALIVE, on ? 1 : 0);
    }

    bool setReuseAddress(bool on) {
        return setIntOption(SO_REUSEADDR, on ? 1 : 0);
    }

    /*
     * setSoLinger
     */
    bool setSoLinger (bool doLinger, int seconds);

    /*
     * setTcpNoDelay
     */
    bool setTcpNoDelay(bool noDelay);

    /*
     * setTcpQuickAck
     */
    bool setTcpQuickAck(bool quickAck);

    /*
     * setIntOption
     */
    bool setIntOption(int option, int value);

    /*
     * setTimeOption
     */
    bool setTimeOption(int option, int milliseconds);

    /*
     * if blocked
     */
    bool setSoBlocking(bool on =false);
    /*
     * check if socket be created
     */
    bool checkSocketHandle();
    bool getTcpInfo(struct tcp_info*) const;
    bool getTcpInfoString(char* buf, int len) const;
    /*
     * get socket error
     */
    int getSoError();
    int        _savedErr;
protected:
    int        _socketHandle;    // socket description
    bool       _own;

};
}
}

#endif /*SOCKET_H_*/
