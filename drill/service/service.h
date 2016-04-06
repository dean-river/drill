#ifndef DRILL_SERVICE_SERVICE_H_
#define DRILL_SERVICE_SERVICE_H_

#include <drill/event/event_loop.h>
#include <drill/event/channel.h>
#include <drill/common/logger.h>
#include <drill/common/properties.h>
#include <drill/common/cmdline.h>

#include <signal.h>
#include <sys/signalfd.h>
#include <memory>
#include <functional>
#include <string>

using namespace drill::common;
using namespace drill::event;


namespace drill {
namespace event {

class EventLoop;

}
namespace service {
    
class Service{
    public:
       Service();
       virtual ~Service();
       void addSignal(int sig);
       virtual void setParseOption();
       int main(int argc, char*argv[]);

       bool  isDaemon();
    protected:
        void start();
        void stop();
        virtual void runDaemon();
		virtual bool initLogConf();
		string                _logbase;
		int                   _rollsize;
		int                   _interval;
		Logger::LogLevel      _loglevel;

        
    protected:
		virtual int run() = 0;
		virtual void dispose() = 0; 
		virtual bool userSignalOpt(int sig);
		virtual std::string version();

		void onReadSignal(Time t);
		void defaultSignalOpt(int sig);
       
    protected:
        bool                      _isdaemon;
        bool                      _active; 
        int                       _signalfd;
        string                    _workdir;
        Properties                _conf;

        string                     _confFile;
        string                     _pidFile;

        std::unique_ptr<Channel>        _sigChannel;
        EventLoop                  *_loop;
        
        sigset_t                    _sigmask;
        parser                      _args;
      

};

}

}
#endif
