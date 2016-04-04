#ifndef DRILL_COMMON_NOCOPYABLE_H_
#define DRILL_COMMON_NOCOPYABLE_H_

namespace drill {
namespace common {

class noncopyable {
public:
    noncopyable(){}
   virtual  ~noncopyable(){}
private:
    noncopyable(const noncopyable&);
    noncopyable& operator=(const noncopyable&);
};

}
}
#endif