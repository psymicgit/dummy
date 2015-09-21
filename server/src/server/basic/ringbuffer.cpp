///<------------------------------------------------------------------------------
//< @file:   server\basic\ringbuffer.cpp
//< @author: 洪坤安
//< @date:   2015年8月3日 19:14:46
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "ringbuffer.h"

#include "net/link.h"

#define ALIGN(s) (((s) + 3 ) & ~3) // 使内存按4字节对齐
// #define ALIGN(s) s

void RingBufferBlock::bind(Link *link)
{
	/*
	m_next = NULL;
	m_skip = 0;
	m_link = link;

	if(NULL == link->m_head) {
		link->m_head = this;
		link->m_tail = this;
	} else {
		link->m_tail->m_next = this;
		link->m_tail = this;
	}
	*/
}

RingBufferBlock* RingBuffer::getHead()
{
	return (RingBufferBlock*)(m_data + m_head);
}

RingBufferBlock* RingBuffer::splitBlock(int sum, int size)
{
	int alignLength = ALIGN(sizeof(struct RingBufferBlock) + size);

	// 切割出第一个长度为size的block
	struct RingBufferBlock *head = getHead();
	head->m_length = size;
	head->m_skip = 0;
	head->m_next = NULL;
	head->m_link = NULL;

	// 切割出第二个长度为sum - size的block
	struct RingBufferBlock *next = getNextBlock(head);
	if (next) {
		int nextOffset = getBlockOffset(next);

		// 如果第二块位于末尾，判断空间是否足够再构造一个block
		if (m_size - nextOffset > sizeof(RingBufferBlock)) {
			if (alignLength < sum) {
				next->m_length = sum - alignLength - sizeof(RingBufferBlock);
				next->m_link = NULL;
				next->m_next = NULL;
				next->m_skip = 0;

				if (next->m_length <= 0) {
					LOG_ERROR << "next->m_length = " << next->m_length;
					statistic();
				}
			}
		}
	} else {
		// m_head = 0;
	}

	// 返回第一块block
	return head;
}

RingBufferBlock* RingBuffer::searchFreeBlock(int size)
{
	int alignLength = ALIGN(sizeof(struct RingBufferBlock) + size);
	int freeSpace = 0;

	// 从head开始依次往后查找空闲block，直到拼接起来的空闲block长度大于传入的size
	struct RingBufferBlock *block = getHead();
	do {
		// 如果该块当前仍在被使用，则中止搜索
		if (block->m_link != NULL && block->m_length >= sizeof(struct RingBufferBlock)) {
			return NULL;
		}

		freeSpace += ALIGN(block->m_length) + sizeof(RingBufferBlock);

		// 检测长度是否已足够分配
		if (freeSpace > alignLength + sizeof(RingBufferBlock)) {
			// 将找到的这几块空闲block切割为2块block，并返回第一块block的地址
			return splitBlock(freeSpace , size);
		}

		block = getNextBlock(block);
	} while(block);

	return NULL;
}

RingBufferBlock* RingBuffer::allocFreeBlock(int size)
{
	// 先从当前位置往后找起
	RingBufferBlock *block = searchFreeBlock(size);

	// 如果找不到则从头开始找
	if (NULL == block) {
		LOG_WARN << "try to find from head = 0";
		statistic();

		m_head = 0;
		block = searchFreeBlock(size);
	}

	// m_head = getBlockOffset(getNextBlock(block));
	return block;
}

void RingBuffer::freeBlock(RingBufferBlock *block)
{
	while(block != NULL) {
		block->m_link = NULL;
		block = block->m_next;
	}
}

RingBufferBlock* RingBuffer::add(const char* data, int len, Link *link)
{
	/*
	if (0 == len) {
		return NULL;
	}

	RingBufferBlock *block = allocFreeBlock(len);
	if (NULL == block) {
		LOG_ERROR << "alloc block fail: len = " << len;
		//statistic();
		//exit(0);

		return NULL;
	}

	block->m_next = NULL;
	block->m_skip = 0;
	block->m_link = link;

	if(NULL == link->m_head) {
		link->m_head = block;
		link->m_tail = block;
	} else {
		link->m_tail->m_next = block;
		link->m_tail = block;
	}

	char *start = m_data + getBlockOffset(block) + sizeof(RingBufferBlock);
	memcpy(start, data, len);

	// m_head = getBlockOffset(getNextBlock(block));

	return block;
	*/

	return NULL;
}

void RingBuffer::skip(int len)
{
	m_head += len;

	if (m_head >= m_size) {
		m_head = 0;

		LOG_WARN << "restart";
		statistic();
		exit(0);
	}
}

void RingBuffer::statistic()
{
	/*
	int usingBlockCount = 0;
	int firstUsingBlockIdx = 0;
	int firstUsingBlockOffset = 0;
	int totalBlockCount = 0;
	int totalSkip = 0;
	int totalFree = 0;

	RingBufferBlock *head = NULL;
	RingBufferBlock *tail = NULL;

	RingBufferBlock *block = (RingBufferBlock *)m_data;
	while (block) {
		totalBlockCount++;

		if (block->m_link != NULL) {
			usingBlockCount++;

			if (firstUsingBlockIdx == 0) {
				firstUsingBlockIdx = totalBlockCount;
				firstUsingBlockOffset = getBlockOffset(block);
			}

			totalSkip += block->m_skip;
			totalFree += block->size();
		}
		else {
			totalFree += block->m_length;
		}

		head = NULL;
		tail = NULL;

		if (block->m_link) {
			head = block->m_link->m_head;
			tail = block->m_link->m_tail;
		}

		LOG_INFO << "[block idx=" << totalBlockCount << ",offset=" << getBlockOffset(block) << ",size=" << block->m_length
		         << ", skip = " << block->m_skip << ", size = " << block->size() << ", "
		         << block->m_link << "[" << getBlockOffset(head) << "," << getBlockOffset(tail) << "]," << getBlockOffset(block->m_next) << "]";

		block = getNextBlock(block);
	}

	LOG_ERROR << "head = " << m_head << ", size = " << m_size;
	LOG_ERROR << "total block count = " << totalBlockCount;
	LOG_ERROR << "total skip = " << totalSkip;
	LOG_ERROR << "total free = " << totalFree;
	LOG_ERROR << "using block count = " << usingBlockCount;
	LOG_ERROR << "first using block count index = " << firstUsingBlockIdx;
	LOG_ERROR << "first using block count offset = " << firstUsingBlockOffset;

	*/
}

void RingBuffer::linkBlock(RingBufferBlock *head, RingBufferBlock *block)
{
	RingBufferBlock *tail = head;
	while (tail->m_next != NULL) {
		tail = tail->m_next;
	}

	tail->m_next = block;
}

int RingBuffer::getBlockOffset(RingBufferBlock *block)
{
	if (NULL == block) {
		return 0;
	}

	char * start = m_data;
	return (char *)block - start;
}

RingBufferBlock* RingBuffer::getNextBlock(RingBufferBlock *block)
{
	if (NULL == block) {
		return NULL;
	}

	int alignLength = ALIGN(block->m_length);
	int offset = getBlockOffset(block);

	int nextOffset = offset + sizeof(RingBufferBlock) + alignLength;
	if (nextOffset >= m_size) {
		return NULL;
	}

	return (RingBufferBlock*)(m_data + nextOffset);
}