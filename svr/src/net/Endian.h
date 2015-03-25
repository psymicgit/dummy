#ifndef Endian_h__
#define Endian_h__

#include <boost/cstdint.hpp>

using namespace boost;

namespace endian
{
    // 将64位的整数从主机字节顺序转换成网络字节顺序
    uint64_t hostToNetwork64(uint64_t host64);

    // 将32位的整数从主机字节顺序转换成网络字节顺序
    uint32_t hostToNetwork32(uint32_t host32);

    // 将16位的整数从主机字节顺序转换成网络字节顺序
    uint16_t hostToNetwork16(uint16_t host16);

    // 将64位的整数从网络字节顺序转换成主机字节顺序
    uint64_t networkToHost64(uint64_t net64);

    // 将32位的整数从网络字节顺序转换成主机字节顺序
    uint32_t networkToHost32(uint32_t net32);

    // 将16位的整数从网络字节顺序转换成主机字节顺序
    uint16_t networkToHost16(uint16_t net16);
}

#endif // Endian_h__
