

#ifndef _SDF_CLISESSION_H_
#define _SDF_CLISESSION_H_

#include <sdsingleton.h>
#include <protocol/server/protocommondata.h>

using namespace std ;

class CPlayer;

enum EUserStat
{
    CLIS_LOGIN_INVALID        = -1,
    CLIS_GET_USER_DATA_ING    ,//向db发送请求用户数据中
    CLIS_GET_USER_DATA_COMPLETE    ,//获得db的用户应答数据包
    CLIS_WAIT_CLI_LOAD_FINISH    ,//等待客户端load数据完成中
    CLIS_IN_GAME                ,//玩家正常进入游戏
	CLIS_DISCONNECT                ,//玩家断开连接
    CLIS_ALL           , //用于描述所有状态
};


class CUserMgr;
/**
* @brief 客户端连接类
* 接收到网关汇报，有客户端接入时构造该类实例
* 接收到网关汇报，有客户端离开时释放该类实例
*/
class CUser
{
	friend class CUserMgr;
public:
    CUser();
    ~CUser();
public:
    /**
    * @brief
    * 向客户端连接发送数据
    * @param pData :  数据内容
    * @param dwLen :  数据长度
    * @return FALSE : 发送失败 ，TRUE : 发送成功
    **/
    BOOL            Send(const CHAR* pData, UINT32 dwLen);

    /**
    * @brief
    * 向客户端连接发送消息结构体
    * @param pData :  数据内容
    * @param wMsgID :  消息类型
    * @return FALSE : 发送失败 ，TRUE : 发送成功
    **/
    BOOL            SendMsg(const CHAR* pData, UINT16 wMsgID);


    /**
    * @brief
    * 获取当前的用户在GS上的状态
    * 用户可能正在向DBServer取角色数据等
    * @return 用户当前状态
    **/
    EUserStat       GetUserStat();

    /**
    * @brief
    * 获取当前的用户在GS上的状态
    * 用户可能正在向DBServer取角色数据等
    * @param enUserStat :  用户当前状态
    * @return 用户当前状态
    **/
    VOID            SetUserStat(EUserStat enUserStat);

	    /**
    * @brief
    * 获取当前的用户玩家
    * @return 用户玩家
    **/
    //CPlayer*    GetPlayer() { return m_poPlayer; }

	CPlayer*    GetPlayer();

	/**
    * @brief
    * 保存玩家数据
    * @return 用户玩家
    **/
    //BOOL		SavePlayerData();

	VOID			SetDisconnectTime(UINT64 qwDisconnectTime = SDTimeSecs()) { m_qwDisconnectTime = qwDisconnectTime; }
	UINT64		GetDisconnectTime() { return m_qwDisconnectTime; }

	UINT32		GetUserSessionID() { return m_dwPlayerID; }
	UINT32		GetPlayerID() { return m_dwPlayerID; }
	UINT32		GetUserID() { return m_dwUserID; }
	UINT16		GetCareerID() { return m_wCareerID; }
	
	TCHAR*		GetUserName() { return m_szUserName; }
	TCHAR*		GetDeviceID() { return m_szDeviceID; }
	TCHAR*		GetNotifyID() { return m_szNotifyID; }
    UINT16		GetZoneID() { return m_wZoneID; }
	UINT16		GetOriginalZoneID() { return m_wOriZoneID; }
	UINT32		GetLoginIP() { return m_dwLoginIP; }
	UINT32		GetCliVer() { return m_dwCliVer; }
	UINT8		GetAuthType() { return m_byAuthType; }
	UINT8		GetDaiChong() { return m_byDaiChong; }

	VOID			CkUserLogin();
	VOID			CkUserDisconnect();

	VOID			SetUserProp(UINT32 dwUserID, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wCareerID, UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwLoginIP, UINT32 dwCliVer, UINT8 byAuthType, UINT8 byDaiChong);
private:
    EUserStat       m_enUserStat;//当前Session的状态
	
	UINT32			m_dwUserID;//用户ID
	UINT32			m_dwPlayerID;//用户会话ID用PlayerID
	
	UINT16			m_wCareerID;//职业ID，新创建用户有效
    UINT16			m_wZoneID;//区ID
	UINT16			m_wOriZoneID;//玩家创建角色时所在的区ID
	UINT32			m_dwLoginIP;//登录IP
	UINT32			m_dwCliVer;//客户端版本
	UINT8			m_byAuthType;
	TCHAR			m_szUserName[USERNAME_LEN];
	TCHAR			m_szDeviceID[DEVICEID_LEN];
	TCHAR			m_szNotifyID[NOTIFYID_LEN];
	UINT64			m_qwDisconnectTime;	//短线时间,用于过期清除
	UINT64			m_qwLastRecvMsgTime; //上次收到信息时间
	UINT8			m_byDaiChong;
};


#define ZERO_PACKET(PKT_ACK)\
	ZeroPacketBuffer(sizeof(PKT_ACK));\
	PKT_ACK* pstAck = (PKT_ACK*)(GetPacketBuffer());


#endif //#ifndef _SDF_CLISESSION_H_

