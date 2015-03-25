#ifndef ProtobufApi_h__
#define ProtobufApi_h__

#include <boost/cstdint.hpp>

using namespace boost;

namespace protobufapi
{
    #define HEADER_LENGTH (2 * sizeof int16_t)
    #define TAIL_LENGTH sizeof uint32_t

    // 校验消息包，校验匹配返回true，不匹配返回false，说明该消息有误
    bool checksum(const char* msg, size_t len);

    uint32_t adler32(const unsigned char *data, int32_t len);
}

#endif // ProtobufApi_h__
