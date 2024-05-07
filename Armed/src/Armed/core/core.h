#pragma once
#include<memory>

#ifdef ARM_DEBUG
    #define ARM_ASSERT(x,y) if(!(x)) {std::cout << y << '\n'; __debugbreak();}
    #define ARM_ERROR(x) {std::cout << x << '\n';}
#else
    #define ARM_ASSERT(x,y)
    #define ARM_ERROR(x)
#endif // ARM_DEBUG

#define ARM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Arm {
    template <typename T>
    using Scope = std::unique_ptr<T>;
    template <typename T>
    using Ref = std::shared_ptr<T>;
}