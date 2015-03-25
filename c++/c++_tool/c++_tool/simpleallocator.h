/********************************************************************
            Copyright (c) 2009, 欢乐连线工作室
                   All rights reserved
         
    创建日期：  2009年12月10日 10时45分
    文件名称：  SimpleAllocator.h
    说    明：  简单的内存分配器
	            注: 实现参考了Tom Gambill, NCsoft的设计，并在其基础上进行
				    了优化和修正
    
    当前版本：  1.00
    作    者：  Conserlin
    概    述：  创建  

*********************************************************************/
#pragma once
#include <vector>

using namespace std;

template <
		 typename T,              // 内存分配的类型
         size_t blocksPerBatch    // 每次开辟多少个T内存空间
         >
class SimpleAllocator
{
public:
	typedef vector<T*> TVector;
	typedef vector<unsigned char*> ByteVector;

public:
	SimpleAllocator(): nextAllocation(0) {
	}

	~SimpleAllocator();

	// 申请一个T大小的内存空间
	T* allocateBlock();
	// 释放
	void releaseBlock(T *pBlock);

private:
	// 用于存储申请到的T空间
	TVector objectVector;
	// 用于存储申请到的空间地址，便于释放
	ByteVector batchVector;
	// 记录下一个T申请的位置
	size_t nextAllocation;
};

template <typename T, size_t blocksPerBatch>
SimpleAllocator<T, blocksPerBatch>::~SimpleAllocator() {
	//return;
	size_t iNum = batchVector.size();
	for (size_t i = 0; i < iNum; ++i) {
		unsigned char *p = batchVector[i];
		delete []p;
	}
}

template <typename T, size_t blocksPerBatch>
void SimpleAllocator<T, blocksPerBatch>::releaseBlock(T *pBlock) {
	if (pBlock) {
		objectVector[--nextAllocation] = pBlock;
	}		
}

template <typename T, size_t blocksPerBatch>
T* SimpleAllocator<T, blocksPerBatch>::allocateBlock() {
	if (nextAllocation >= objectVector.size()) {
		size_t st_bpb = 0 == blocksPerBatch ? 1 : blocksPerBatch;
		size_t size = sizeof(T) * st_bpb;

		unsigned char *pBatch = new unsigned char[size];		
		batchVector.push_back(pBatch);
		
		size = objectVector.size() + st_bpb;
		objectVector.resize(size);

		size_t iNew = nextAllocation;
		for (size_t i = 0; i < st_bpb; ++i) {
			objectVector[iNew++] = (T*)&(pBatch[sizeof(T) * i]);
		}	
	}

	return objectVector[nextAllocation++];
}
