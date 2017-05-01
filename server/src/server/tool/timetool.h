///<------------------------------------------------------------------------------
//< @file:   server\tool\timetool.h
//< @author: ������
//< @date:   2015��1��16�� 22:54:23
//< @brief:	 ��ȡʱ��
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _timetool_h_
#define _timetool_h_

typedef int64 Timestamp; // utc��ʽʱ�䣬��1900�𾭹���ʱ�䣬��λ����
typedef int32 TimeInMs; // in ms���뵥λ

#define MsPerSecond 1000 // ����
#define UsPerSecond (1000 * 1000) // ΢��

namespace timetool
{
	// ��ȡ��ǰʱ�䣬��λ: ����
	Timestamp now();

	// ��ȡ����ʱ��ʱ�䣬��λ: ����
	Timestamp localnow();

	const char* timeToText(const Timestamp& time);

	const char* nowText();
}

#endif //_timetool_h_