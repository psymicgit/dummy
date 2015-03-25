#include "ProtobufApi.h"

#include <net/Endian.h>

namespace
{
    const int MOD_ADLER = 65521;

    uint16_t asNetInt16(const char* buf)
    {
        uint16_t be16 = 0;
        ::memcpy(&be16, buf, sizeof(be16));
        return endian::networkToHost16(be16);
    }

    uint32_t asNetInt32(const char* buf)
    {
        uint32_t be32 = 0;
        ::memcpy(&be32, buf, sizeof(be32));
        return be32;
    }
}

namespace protobufapi
{
    // 消息包的格式是
    // | 后面包长 | 消息类别 | 消息内容 | 校验码 |
    // | 2 Byte   | 2 Byte   | ～～～～ | 2 Byte |
    bool checksum(const char* msg, size_t len)
    {
        uint32_t expectedCheckSum = asNetInt32(msg + len - TAIL_LENGTH);
        uint32_t checkedSum = adler32((const unsigned char*)msg, len - TAIL_LENGTH);
        if(expectedCheckSum != checkedSum)
        {
            return false;
        }

        return true;
    }

    uint32_t adler32(const unsigned char *data, int32_t len) /* where data is the location of the data in physical memory and 
                                                           len is the length of the data in bytes */
    {
        uint32_t a = 1, b = 0;
        int32_t index;
     
        /* Process each byte of the data in order */
        for (index = 0; index < len; ++index)
        {
            a = (a + data[index]) % MOD_ADLER;
            b = (b + a) % MOD_ADLER;
        }
     
        return (b << 16) | a;
    }
}
