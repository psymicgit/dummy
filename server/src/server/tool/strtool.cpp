#include "strtool.h"

#include <rapidjson/internal/itoa.h>

namespace socktool
{
	std::string itoa(int32 n)
	{
		char buf[12];
		rapidjson::internal::i32toa(n, buf);
		return buf;
	}

	std::string itoa(int64 n)
	{
		char buf[12];
		rapidjson::internal::u32toa(n, buf);
		return buf;
	}

	std::string itoa(uint32 n)
	{
		char buf[30];
		rapidjson::internal::i64toa(n, buf);
		return buf;
	}

	std::string itoa(uint64 n)
	{
		char buf[30];
		rapidjson::internal::u64toa(n, buf);
		return buf;
	}
}