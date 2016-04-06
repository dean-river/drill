#include <drill/net/acceptor.h>
#include <drill/event/event_loop.h>
#include <drill/net/addr_inet.h>
#include <drill/common/logger.h>

#include <functional>
#include <cerrno>
#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h>

using namespace drill::common;
using namespace drill::net;
using namespace drill::event;

Acceptor::Acceptor(EventLoop* loop,  AddrInet& listenAddr, bool reuseport)
  : _loop(loop),
  _acceptSocket(true),
    _listenning(false),
    _idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
  assert(_idleFd >= 0);
  if(!_acceptSocket.bind(listenAddr)) {

	LOG_ERROR<<"addr in use can not bind";
  	abort();
	
  }
  _acceptSocket.setReuseAddress(reuseport);
  _acceptChannel.setUp(_loop,_acceptSocket.getFd());
  _acceptChannel.setReadCallback(
  std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
  _acceptChannel.disableAll();
  _acceptChannel.remove();
  ::close(_idleFd);
}

void Acceptor::listen()
{
  _loop->assertInLoopThread();

  bool rc = _acceptSocket.listen(SOMAXCONN);
  if(rc == false ) {
	LOG_ERROR<<"bind and listen addr error";
	assert(false);
  }
  _listenning = true;
  _acceptChannel.enableReading();
}

void Acceptor::handleRead()
{
	_loop->assertInLoopThread();
  	AddrInet peerAddr;
  	int connfd;
  	do {
  		connfd = _acceptSocket.accept(peerAddr);
	  	if (connfd >= 0) {
	    	string hostport = peerAddr.toString();
	    	LOG_TRACE << "Accepts of " << hostport;
	    	if (_newConnectionCallback) {
	     		 _newConnectionCallback(connfd, peerAddr);
	    	} else{
	      		::close(connfd);
	    	}
	  	}else {
			if(errno == EAGAIN){
				break;
			}    
	    // Read the section named "The special problem of
	    // accept()ing when you can't" in libev's doc.
	    // By Marc Lehmann, author of livev.
	    if (errno == EMFILE) {
	      ::close(_idleFd);
	      _idleFd = ::accept(_acceptSocket.getFd(), NULL, NULL);
	      ::close(_idleFd);
	      _idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
		  break;
	    }
		LOG_ERROR<<"in Acceptor::handleRead";
	  }
	}while(connfd > 0);
}

