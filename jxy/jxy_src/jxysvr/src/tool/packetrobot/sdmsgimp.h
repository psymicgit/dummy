#include "sdmsg.h"
#include <map>
#include <string>

using namespace std;
using namespace SGDP;

struct SValue
{
	CHAR*			pszValue;
	UINT32			nValueLen;
};

class CSDMsgImp: public ISDMsg
{
	typedef std::map<std::string, SValue> MapKeyValue;
	typedef MapKeyValue::iterator ItMapKeyValue;

	typedef allocator<CHAR> _Ax;
	_Ax::template rebind<CHAR>::other _Alnod;
private:
	~CSDMsgImp();
public:
	CSDMsgImp();
	BOOL SDAPI PushValue(const CHAR* pszKey, const UINT8* pData, UINT32 nLen);
	INT32 SDAPI GetValue(const CHAR* pszKey, UINT8* pValue, UINT32 &nLen);
	BOOL SDAPI PushBuffer(const UINT8* pBuf, UINT32 nLen);
	INT32 SDAPI GetBuffer(UINT8* pDestBuf, UINT32 &nLen);
	VOID SDAPI CleanData();
	INT32 SDAPI CopyData(ISDMsg* pSrcMsg);
	VOID SDAPI Release();
private:
	MapKeyValue m_mapKeyValue;
};

