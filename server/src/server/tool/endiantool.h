///<------------------------------------------------------------------------------
//< @file:   server\tool\endiantool.h
//< @author: ������
//< @date:   2014��11��25�� 15:26:49
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _endiantool_h_
#define _endiantool_h_

namespace endiantool
{
	// �����ֽ���ת���������ֽ���64λ����
	uint64_t HostToNetwork64(uint64_t host64);

	// ��32λ�������������ֽ�˳��ת���������ֽ�˳��
	uint32 HostToNetwork32(uint32 host32);

	// ��16λ�������������ֽ�˳��ת���������ֽ�˳��
	uint16 HostToNetwork16(uint16 host16);

	// ��64λ�������������ֽ�˳��ת���������ֽ�˳��
	uint64_t NetworkToHost64(uint64_t net64);

	// ��32λ�������������ֽ�˳��ת���������ֽ�˳��
	uint32 NetworkToHost32(uint32 net32);

	// ��16λ�������������ֽ�˳��ת���������ֽ�˳��
	uint16 NetworkToHost16(uint16 net16);
}

#endif //_endiantool_h_