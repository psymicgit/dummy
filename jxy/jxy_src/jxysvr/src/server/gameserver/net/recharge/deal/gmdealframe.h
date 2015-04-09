// DealFrame.h: interface for the CRsDealFrame class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MSDEALFRAME_H_
#define _MSDEALFRAME_H_

#include <sdtype.h>
#include <map>
#include <vector>
#include <string>
#include "common/client/gmdef.h"
#include "common/client/errdef.h"
#include "common/server/utility.h"
using namespace std;



class CRCClient;


namespace InterfaceDealBase
{

class CGmManager;

class CGmDealFrame
{
public:

    CGmDealFrame();
    CGmDealFrame(CGmManager * IN_pManager);
    virtual ~CGmDealFrame();
    virtual void Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc) = 0;   //处理协议数据
	virtual void Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField) = 0;
	virtual void   Interface(CRCClient* poClinet) = 0;
    virtual UINT16 GetFrameType() = 0;          //获取该协议类返回的协议类型号
	virtual string GetStrFrameType() = 0;          //获取该协议类返回的协议类型号
	virtual string GetStrFrameDesc() = 0;
    virtual void Release() ;               //释放对象，提供外部
    virtual bool init(CGmManager * IN_pManager);//初始化协议对象本身

public:
	typedef map<string,string>::iterator	MapFiledIter;//用来保存发送到客户端的命令信息 
	MapFiledIter iter;

public:
	void RsponeMsg(CRCClient* poClient,map<string,string> &mapFiled);//将命令处理成JSON格式
	//string GetErrorMsgJson(const UINT16 unErrorType, map<string,string> *mapJson = NULL);//作为一个公共接口，传入错误类型，获得JSON格式的字符串

public:
    CGmManager * m_pManager;
protected:
    void RegisterDealFrame(CGmDealFrame *v_pDealFrame) ;      //上行协议数据处理转换出，协议类注册函数
};

//管理器接口
class CGmManager
{
    //方法
public:
    CGmManager();
    ~CGmManager();

    void				RegisterDealFrame(CGmDealFrame * IN_pDealFrame) ;
    CGmDealFrame*		GetInstanceByType(UINT16 wProto);
	CGmDealFrame*		GetInstanceByType(string strProto);
    CGmManager*			instance();   //获取本实例对象指针
    void				Release();
	void				Deal(UINT16 wProto, CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5, TCHAR *pszDesc);
	void				Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField,string strCmdInfo);
	void				Interface(string strProto, CRCClient* poClinet);
	string				GetGmInterfaceCmd(const string strProto);

private:
    map<UINT16, CGmDealFrame*> m_mapTask;						//存储协议对象指针链表
	map<string, CGmDealFrame*> m_mapStrTask;						//存储协议对象指针链表
	typedef map<UINT16, CGmDealFrame*>::iterator   MapTaskIter;   //
	typedef map<string, CGmDealFrame*>::iterator   MapStrTaskIter;   //
    CGmManager*               m_pInstance;						//单健对象指针，存储自己实例对象的指针

};

}
#endif // !defined(AFX_DEALFRAME_H__03868AAA_9098_4543_9A8C_25E2B88CBD7B__INCLUDED_)
