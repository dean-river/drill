#ifndef DRILL_COMMON_TYPES_H_
#define DRILL_COMMON_TYPES_H_

namespace drill {
namespace common {

template<typename To, typename From>
inline To implicit_cast(From const &f) 
{  
        return f;
}

}

}
#endif
