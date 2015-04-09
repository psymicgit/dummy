// DealFrame.h: interface for the CRsDealFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _RSDEALFRAME_H_
#define _RSDEALFRAME_H_

#include <sdtype.h>
#include <map>
#include <vector>
#include <string>
#include "common/server/utility.h"

#define RESOPTYPE_CKNUM		1	//检查数量
#define RESOPTYPE_DECNUM		2	//减少数量
#define RESOPTYPE_ADDNUM		3	//增加数量
#define RESOPTYPE_CK_ADD_LIMIT	4  //检查添加的数量是否超过限制

struct tagDT_PLAYER_RES_DATA;
typedef tagDT_PLAYER_RES_DATA DT_PLAYER_RES_DATA;

using namespace std;

class CPlayer;

namespace InterfaceDealBase
{
class CRsManager;
class CRsDealFrame
{
public:

    CRsDealFrame();
    CRsDealFrame(CRsManager * IN_pManager);
    virtual ~CRsDealFrame();
    virtual BOOL Deal(CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1 = 0, UINT32 dwNum2 = 0, UINT32 dwNum3 = 0, TCHAR *pszDesc = NULL, DT_PLAYER_RES_DATA* pstResInfo1 = NULL, DT_PLAYER_RES_DATA* pstResInfo2 = NULL) = 0;   //处理车台上行的协议数据
    virtual string GetFrameType() = 0;          //获取该协议类返回的协议类型号
    virtual void Release() ;               //释放对象，提供外部
    virtual bool init(CRsManager * IN_pManager);//初始化协议对象本身
public:
	bool	CkAddLimit(string strKey,UINT32 unAddNum);

public:
    CRsManager * m_pManager;
protected:
    void RegisterDealFrame(CRsDealFrame *v_pDealFrame) ;      //上行协议数据处理转换出，协议类注册函数
};

//管理器接口
class CRsManager
{
    //方法
public:
    CRsManager();
    virtual ~CRsManager();
    void RegisterDealFrame(CRsDealFrame * IN_pDealFrame) ;
    CRsDealFrame * GetInstanceByType(string strKey);
    virtual CRsManager* instance();   //获取本实例对象指针
    virtual void Release();
protected:
    map<string, CRsDealFrame*> m_mapTask;                //存储协议对象指针链表
    typedef map<string, CRsDealFrame*>::iterator   MapTaskIter;   //
    CRsManager*                   m_pInstance;  //单健对象指针，存储自己实例对象的指针
public:
    virtual BOOL Deal(string strKey, CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1 = 0, UINT32 dwNum2 = 0, UINT32 dwNum3 = 0, TCHAR *pszDesc = NULL, DT_PLAYER_RES_DATA* pstResInfo1 = NULL, DT_PLAYER_RES_DATA* pstResInfo2 = NULL);
	string GetAddLimitMsg(string strKey);
};

}
#endif // !defined(AFX_DEALFRAME_H__03868AAA_9098_4543_9A8C_25E2B88CBD7B__INCLUDED_)
