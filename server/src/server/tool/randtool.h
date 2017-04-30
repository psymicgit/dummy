///<------------------------------------------------------------------------------
//< @file:   server\tool\randtool.h
//< @author: ������
//< @date:   2015��3��17�� 22:40:38
//< @brief:	 ��ȡ�����
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _randtool_h_
#define _randtool_h_

typedef std::vector<int> HitVec;

namespace randtool
{
	// ��ʼ����������ӣ�����������ʱ����
	void initSeed();

	// ��ȡ�����
	int rand_int_all();

	// ��ȡ���������Χ��[0, n - 1]
	int rand_int(int n);

	// ��ȡ���������Χ��[a, b]
	int rand_int_between(int a, int b);

	// ��ȡ���������
	float rand_float(float n);

	// ��ȡ���������
	float rand_float_between(float a, float b);

	// ���ݸ��ʱ����������������еĸ������(hitVec�е�˳��),û�и��ʷ���0���
	int rollHit(HitVec &hitVec);

	// ��ȡһ������������ڼ��ܵ�α�������������Cryptographically Secure Pseudo-Random Number Generator �C CSPRNG��
	void secureRandom(uint8 nums[], int cnt, int beg, int end);
}

#endif //_randtool_h_