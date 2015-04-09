#include "msmqwrapper.h"

#ifdef SGDP_HAS_MSMQ_LOGGER 

CMsmqWarpper::CMsmqWarpper()
 :m_hQueue(NULL), m_sendtype(MQMSG_DELIVERY_EXPRESS)
{

}

CMsmqWarpper::~CMsmqWarpper()
{

}

bool CMsmqWarpper::Init(const char* szPathName, int sendtype /* =0 */)
{
	if (szPathName == NULL)
	{
		return false;
	}

	if (sendtype != 0)
	{
		m_sendtype = MQMSG_DELIVERY_RECOVERABLE;
	}

	WCHAR wszPathName[1024];
	int ret = ::MultiByteToWideChar(CP_ACP, 
									0, 
									szPathName, 
									-1, 
									wszPathName, 
									1024 
									);
	if (ret == 0)
	{
		return false;
	}

	//if (!CreatePublicQueue(wszPathName))
	//{
	//	return false;
	//}

	bool connecttype = (strchr(szPathName, ':') == NULL);
	if (!OpenPublicQueue(wszPathName, connecttype))
	{
		return false;
	}

	return true;
}

void CMsmqWarpper::Release()
{
	ClosePublicQueue();
}

bool CMsmqWarpper::CreatePublicQueue(LPCWSTR wszPathName)
{
	if (wszPathName == NULL)
	{
		return false;
	}

	HRESULT hr = MQ_OK;

	//只设置一个属性，路径
	const int NUM_PROPERTIES = 1;

	MQQUEUEPROPS   QueueProps;
	MQPROPVARIANT  aQueuePropVar[NUM_PROPERTIES];
	QUEUEPROPID    aQueuePropId[NUM_PROPERTIES];
	HRESULT        aQueueStatus[NUM_PROPERTIES];

	int nPropIds = 0;
	aQueuePropId[nPropIds] = PROPID_Q_PATHNAME;
	aQueuePropVar[nPropIds].vt = VT_LPWSTR;
	aQueuePropVar[nPropIds].pwszVal = (LPWSTR)wszPathName;
	nPropIds++;

	QueueProps.cProp = nPropIds;               
	QueueProps.aPropID = aQueuePropId;        
	QueueProps.aPropVar = aQueuePropVar;     
	QueueProps.aStatus = aQueueStatus;

	DWORD dwFormatNameBufferLength = 0;
	hr = MQCreateQueue(NULL,
						&QueueProps,					
						NULL,
						&dwFormatNameBufferLength);	

	// 如果创建成功或者队列已经存在，认为正确
	if (hr == MQ_OK || 
		hr == MQ_ERROR_QUEUE_EXISTS ||
		hr == MQ_INFORMATION_FORMATNAME_BUFFER_TOO_SMALL)
	{
		return true;
	}

	return false;

}

bool CMsmqWarpper::OpenPublicQueue(LPCWSTR wszPathName, bool connecttype)
{
	if (wszPathName == NULL)
	{
		return false;
	}

	HRESULT hr = MQ_OK;

	if (connecttype)
	{
		DWORD dwFormatNameBufferLength = 256; 
		WCHAR wszFormatNameBuffer[256] = {0};

		hr = MQPathNameToFormatName(wszPathName,
			wszFormatNameBuffer,
			&dwFormatNameBufferLength);
		if (FAILED(hr))
		{
			return false;
		}

		hr = MQOpenQueue(wszFormatNameBuffer, 
			MQ_SEND_ACCESS, 
			MQ_DENY_NONE, 
			&m_hQueue
			);
	}
	else
	{
		hr = MQOpenQueue(wszPathName, 
			MQ_SEND_ACCESS, 
			MQ_DENY_NONE, 
			&m_hQueue
			);
	}
	if (FAILED(hr))
	{
		return false;
	}

	return true;

}

void CMsmqWarpper::ClosePublicQueue()
{
	if (m_hQueue == NULL)
	{
		MQCloseQueue(m_hQueue);
		m_hQueue = NULL;
	}
}

bool CMsmqWarpper::SendMessage(const char* pdata, int len)
{
	const int NUM_OF_PROPERTIES = 1; 
	int nPropId = 0;   
	HRESULT hr = MQ_OK; 

	MQMSGPROPS msgProps;
	MSGPROPID aMsgPropId[NUM_OF_PROPERTIES];
	MQPROPVARIANT aMsgPropVar[NUM_OF_PROPERTIES];
	HRESULT aMsgStatus[NUM_OF_PROPERTIES];

	aMsgPropId[nPropId] = PROPID_M_BODY;               
	aMsgPropVar[nPropId].vt = VT_VECTOR | VT_UI1; 
	aMsgPropVar[nPropId].caub.pElems = (unsigned char*)(pdata);
	aMsgPropVar[nPropId].caub.cElems = len;
	nPropId++;

	msgProps.cProp = nPropId;
	msgProps.aPropID = aMsgPropId;
	msgProps.aPropVar = aMsgPropVar;
	msgProps.aStatus = aMsgStatus;

	hr = MQSendMessage(m_hQueue,
						&msgProps,
						MQ_NO_TRANSACTION);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}


#endif // 

