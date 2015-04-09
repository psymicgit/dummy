#ifndef __SDF_CLI_HANDLER_INCLUDE_H_
#define __SDF_CLI_HANDLER_INCLUDE_H_
#include "sdframework.h"
#include "protocol/server/protocligt.h"

class CCliProcessor : public CSDPacketProcessor
{
public:
    CCliProcessor();
public:
    ~CCliProcessor();
    virtual BOOL Init();
    virtual CSDProtocol* GetProtocol();

protected:
    //处理进入游戏消息
    static BOOL			OnEnterGsReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理注册帐号消息
    static BOOL			OnRegisterReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理认证消息
    static BOOL			OnLoginExReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);




    //处理用户绑定消息
    static BOOL			OnBindReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理用户绑定消息
    static BOOL			OnUnBindReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理登出消息
    static BOOL			OnLogoutReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理修改消息
    static BOOL			OnChangePwdReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理修改消息
    static BOOL			OnBindMobileReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理修改消息
    static BOOL			OnBindCheckVerCodeReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理修改消息
    static BOOL			OnUnBindMobileReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理修改消息
    static BOOL			OnUnBindCheckVerCodeReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);

    //处理修改消息
    static BOOL			OnGetUserExpandReq (VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen);
private:

};

#endif

