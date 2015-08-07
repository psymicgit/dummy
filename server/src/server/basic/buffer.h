///<------------------------------------------------------------------------------
//< @file:   server\basic\buffer.h
//< @author: 洪坤安
//< @date:   2015年2月26日 18:32:56
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _buffer_h_
#define _buffer_h_

#include <string.h>

#ifdef WIN
	#pragma warning( disable : 4996 )
#endif

#include "tool/endiantool.h"

/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode
class Buffer
{
public:
	static const size_t g_cheapPrepend;
	static const size_t g_initSize;
	static const char kCRLF[];

	explicit Buffer(size_t initialSize = g_initSize)
		: m_buffer(g_cheapPrepend + initialSize),
		  readerIndex_(g_cheapPrepend),
		  writerIndex_(g_cheapPrepend)
	{
		assert(readableBytes() == 0);
		assert(writableBytes() == initialSize);
		assert(prependableBytes() == g_cheapPrepend);
	}

	// implicit copy-ctor, move-ctor, dtor and assignment are fine
	// NOTE: implicit move-ctor is added in g++ 4.6

	void swap(Buffer& rhs)
	{
		m_buffer.swap(rhs.m_buffer);
		std::swap(readerIndex_, rhs.readerIndex_);
		std::swap(writerIndex_, rhs.writerIndex_);
	}

	inline size_t readableBytes() const { return writerIndex_ - readerIndex_; }

	inline size_t writableBytes() const { return m_buffer.size() - writerIndex_; }

	inline size_t prependableBytes() const { return readerIndex_; }

	inline const char* peek() const { return begin() + readerIndex_; }

	inline bool empty() const { return readableBytes() == 0; }

	// retrieve returns void, to prevent
	// string str(retrieve(readableBytes()), readableBytes());
	// the evaluation of two functions are unspecified
	void skip(size_t len)
	{
		assert(len <= readableBytes());
		if (len < readableBytes()) {
			readerIndex_ += len;
		}
		else {
			clear();
		}
	}

	void skipTo(const char* end)
	{
		assert(peek() <= end);
		assert(end <= beginWrite());
		skip(end - peek());
	}

	void clear()
	{
		readerIndex_ = g_cheapPrepend;
		writerIndex_ = g_cheapPrepend;
	}

	std::string takeAllAsString()
	{
		return takeAsString(readableBytes());;
	}

	std::string takeAsString(size_t len)
	{
		assert(len <= readableBytes());

		std::string result(peek(), len);
		skip(len);
		return result;
	}

	void append(const char* data, size_t len)
	{
		if (writableBytes() < len) {
			makeSpace(len);
		}
		assert(writableBytes() >= len);

		std::copy(data, data + len, beginWrite());
		hasWritten(len);
	}

	void append(const void* /*restrict*/ data, size_t len)
	{
		append(static_cast<const char*>(data), len);
	}

	char* beginWrite() { return begin() + writerIndex_; }
	const char* beginWrite() const { return begin() + writerIndex_; }

	void hasWritten(size_t len)
	{
		assert(len <= writableBytes());
		writerIndex_ += len;
	}

	void unread(size_t len)
	{
		assert(len <= prependableBytes());
		readerIndex_ -= len;
	}

	void unwrite(size_t len)
	{
		assert(len <= readableBytes());
		writerIndex_ -= len;
	}

	// Append int64_t using network endian
	void appendInt64(int64 x)
	{
		int64 be64 = endiantool::hostToNetwork64(x);
		append(&be64, sizeof be64);
	}

	// Append int32_t using network endian
	void appendInt32(int32 x)
	{
		int32 be32 = endiantool::hostToNetwork32(x);
		append(&be32, sizeof be32);
	}

	void appendInt16(int16 x)
	{
		int16 be16 = endiantool::hostToNetwork16(x);
		append(&be16, sizeof be16);
	}

	void appendInt8(char x)
	{
		append(&x, sizeof x);
	}
	int64 peekInt64() const
	{
		assert(readableBytes() >= sizeof(int64));
		int64 be64 = 0;
		::memcpy(&be64, peek(), sizeof be64);
		return endiantool::networkToHost64(be64);
	}

	int32 peekInt32() const
	{
		assert(readableBytes() >= sizeof(int32));
		int32 be32 = 0;
		::memcpy(&be32, peek(), sizeof be32);
		return endiantool::networkToHost32(be32);
	}

	int16 peekInt16() const
	{
		assert(readableBytes() >= sizeof(int16));
		int16 be16 = 0;
		::memcpy(&be16, peek(), sizeof be16);
		return endiantool::networkToHost16(be16);
	}

	char peekInt8() const
	{
		assert(readableBytes() >= sizeof(char));
		char x = *peek();
		return x;
	}

	void prepend(const void* /*restrict*/ data, size_t len)
	{
		assert(len <= prependableBytes());
		readerIndex_ -= len;
		const char* d = static_cast<const char*>(data);
		std::copy(d, d + len, begin() + readerIndex_);
	}

	size_t capacity() const
	{
		return m_buffer.capacity();
	}

	char* begin()
	{ return &m_buffer[0]; }

	const char* begin() const
	{ return &m_buffer[0]; }

private:
	void makeSpace(size_t len)
	{
		if (writableBytes() + prependableBytes() < len + g_cheapPrepend) {
			// FIXME: move readable data
			m_buffer.resize(writerIndex_ + len);
		}
		else {
			// move readable data to the front, make space inside buffer
			assert(g_cheapPrepend < readerIndex_);
			size_t readable = readableBytes();
			std::copy(begin() + readerIndex_,
			          begin() + writerIndex_,
			          begin() + g_cheapPrepend);
			readerIndex_ = g_cheapPrepend;
			writerIndex_ = readerIndex_ + readable;
			assert(readable == readableBytes());
		}
	}

private:
	std::vector<char> m_buffer;
	size_t readerIndex_;
	size_t writerIndex_;
};

#endif //_buffer_h_