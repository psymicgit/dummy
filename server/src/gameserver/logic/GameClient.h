///<------------------------------------------------------------------------------
//< @file:   gameserver\logic\client.h
//< @author: ������
//< @date:   2015��6��10�� 22:36:13
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _client_h_
#define _client_h_

// �ͻ��ˣ���Ӧ�����ص�һ���������
class GameClient
{
public:
	bool Init();

	// ������Ϣ��
	bool SendMsg(int msgId, Message&);

	// ������Ϣ��
	bool Send(int msgId, const char* msg, int msgSize);

public:
	int m_clientId;
};

#endif //_client_h_