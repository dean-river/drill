#ifndef DRILL_NET_ADDRINET_H_
#define DRILL_NET_ADDRINET_H_

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstdint>

#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>


namespace drill {
namespace net {

class AddrInet {
public:
	AddrInet();
	AddrInet(const AddrInet& rhs);
    AddrInet(const char*addr);
	AddrInet(const sockaddr_in *addr, int len);
    AddrInet(const char* spip, int ipport);

	~AddrInet (void);

	bool set(const AddrInet & rhs);

	bool set (const char*addr);

	bool set (const struct sockaddr_in * sa, int len);

	bool set (const char* spip, int ipport);

	bool upDateLocal(int fd);
	struct sockaddr_in* getAddr (void);
	/*
	 * flag if need port
	 */
	std::string toString(bool flag=true) const ;

    uint32_t  port();

	uint64_t ipAndPort();

	uint32_t ip();

	std::string ipStr();

	bool operator == (const AddrInet &rhs) const;

	bool operator != (const AddrInet &rhs) const;

	uint64_t hash (void) const;

private:

	 struct sockaddr_in  _inet_addr;
private:

	 void reset(void);

};
}
}

#endif /* ADDRINET_H_ */
