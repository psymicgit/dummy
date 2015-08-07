///<------------------------------------------------------------------------------
//< @file:   server\basic\ringbuffer.h
//< @author: 洪坤安
//< @date:   2015年8月3日 19:14:52
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _ringbuffer_h_
#define _ringbuffer_h_

class Link;

struct RingBufferBlock {
	RingBufferBlock()
	{
		bzero(this, sizeof(*this));
	}

	int getTotalLength()
	{
		int totalLength = 0;
		RingBufferBlock *block = this;
		while(block) {
			totalLength += block->size();
			block = block->m_next;
		}

		return totalLength;
	}

	char* begin()
	{
		return (char*)this + sizeof(*this) + m_skip;
	}

	int size() { return m_length - m_skip; }

	void take(Buffer &buff, int len)
	{
		RingBufferBlock *block = this;
		while(block) {
			int blockSize = block->size();

			if (len > blockSize) {
				buff.append(block->begin(), blockSize);
				len -= blockSize;
			}
			else {
				buff.append(block->begin(), len);
				break;
			}

			block = block->m_next;
		}
	}

	void take(int len, char *to)
	{
		int start = 0;
		RingBufferBlock *block = this;

		while(block) {
			int blockSize = block->size();

			if (len > blockSize) {
				memcpy(to + start, block->begin(), blockSize);

				len -= blockSize;
				start += blockSize;
			}
			else {
				memcpy(to + start, block->begin(), len);
				break;
			}

			block = block->m_next;
		}
	}

	// 跳跃指定的距离，并返回跳跃结束时的block
	RingBufferBlock* skip(int len)
	{
		RingBufferBlock *block = this;
		RingBufferBlock *next  = NULL;

		int blockSize = 0;

		while(block) {
			blockSize = block->size();
			next = block->m_next;

			if (len >= blockSize) {
				block->m_link = NULL;
				block->m_next = NULL;
				block->m_skip = block->m_length;

				len -= blockSize;
			}
			else {
				block->m_skip += len;
				break;
			}

			block = next;
		}

		return block;
	}

	void bind(Link *link);

	int m_length;
	RingBufferBlock *m_next;
	Link *m_link;

	// 记录本block多少数据应被跳过: skip之前的数据是已被处理过，从skip开始的数据才是未处理的数据
	int m_skip;
};

// RingBuffer的分布类似于
// | 长度 下一个Block 所属连接 数据 | 长度 下一个Block 所属连接 数据 | 长度 下一个Block 所属连接 数据 | ......
class RingBuffer
{
public:
	RingBuffer(int length)
		: m_size(length)
		, m_head(0)
	{
		m_data = new char[m_size];
		bzero(m_data, m_size);

		RingBufferBlock* head = getHead();
		head->m_length = m_size - sizeof(RingBufferBlock);
	}

	~RingBuffer()
	{
		delete[] m_data;
	}

public:
	RingBufferBlock* add(const char* data, int len, Link *link);

	void skip(int len);

	RingBufferBlock* getHead();

	// 将长度总和为sum的1个或连续几个空闲block切割为2个block[size, sum - size]
	RingBufferBlock* splitBlock(int sum, int size);

	// 搜索出大于指定大小的第一个空闲block（连续几个空闲block也算在内）
	RingBufferBlock* searchFreeBlock(int size);

	// 申请出指定大小的空闲block
	RingBufferBlock* allocFreeBlock(int size);

	// 释放block
	void freeBlock(RingBufferBlock*);

	// 将block连接到head队列的尾部，即: head -> next -> next ...... -> next = block
	void linkBlock(RingBufferBlock *head, RingBufferBlock *block);

	// 获取block的偏移
	int getBlockOffset(RingBufferBlock *block);

	// 获取相邻的下一个block起始位置
	RingBufferBlock *getNextBlock(RingBufferBlock *block);

	void statistic();

public:
	int m_size;
	int m_head;

	char *m_data;
};

#endif //_ringbuffer_h_