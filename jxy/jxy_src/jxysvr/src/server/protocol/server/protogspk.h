/*       Version Number: 3          */

#ifndef PROTOGSPK_H_INCLUDE_VERSION_3
#define PROTOGSPK_H_INCLUDE_VERSION_3

#include "netdata.h"
#include "protocommondata.h"
#include "sdtype.h"
#include "sdstring.h"
#pragma pack(push, 1)

/*Defines Begin*/
#define MAX_REQ_SYNC_PLAYER_CNT_ONCE 160
#define MAX_RET_SYNC_PLAYER_CNT_ONCE 1
#define GVG_VIDEO_SYNC_CNT_ONCE  60
#define GVG_PK_VIDEO_RET_CNT_ONCE 1
#define PROTOGSPK_MSGID_RANGE 3119
#define PROTOGSPK_MSGID_OFFSET 0
/*Defines End*/


/*Define Structs and Unions        Begin*/
typedef struct tagDT_SYNC_PLAYER{
	DT_PLAYER_NOPVP_DATA stPlayerNoPvpData; //玩家数据
}DT_SYNC_PLAYER;

typedef struct tagPKT_PKGS_GVG_NEXT_ROUND_NTF{
	tagPKT_PKGS_GVG_NEXT_ROUND_NTF() { memset(this, 0, sizeof(tagPKT_PKGS_GVG_NEXT_ROUND_NTF)); }
	UINT8            byRound; //跨服战将进入到哪个阶段
}PKT_PKGS_GVG_NEXT_ROUND_NTF;

typedef struct tagPKT_PKGS_GVG_GET_TOP_8_REQ{
	tagPKT_PKGS_GVG_GET_TOP_8_REQ() { memset(this, 0, sizeof(tagPKT_PKGS_GVG_GET_TOP_8_REQ)); }
	UINT8            byExt; //扩展字段
}PKT_PKGS_GVG_GET_TOP_8_REQ;

typedef struct tagPKT_GSPK_GVG_RET_TOP_8_ACK{
	tagPKT_GSPK_GVG_RET_TOP_8_ACK() { memset(this, 0, sizeof(tagPKT_GSPK_GVG_RET_TOP_8_ACK)); }
	UINT8            byTop8Cnt; //前8强数
	DT_GVG_FACTION   astTop8List[MAX_GVG_TOP_FACTION_CNT]; //前8强列表
}PKT_GSPK_GVG_RET_TOP_8_ACK;

typedef struct tagPKT_PKGS_GET_FACTIONS_REQ{
	tagPKT_PKGS_GET_FACTIONS_REQ() { memset(this, 0, sizeof(tagPKT_PKGS_GET_FACTIONS_REQ)); }
	DT_ONE_ZONE_FACTION_ID_LIST stFactionIDList; //门派id列表
}PKT_PKGS_GET_FACTIONS_REQ;

typedef struct tagDT_PK_FACTION_LIST{
	UINT8            byFactionCnt; //有多少个门派
	DT_PK_FACTION    astFactionList[MAX_GVG_TOP_FACTION_CNT]; //门派列表
}DT_PK_FACTION_LIST;

typedef struct tagPKT_GSPK_RET_FACTIONS_ACK{
	tagPKT_GSPK_RET_FACTIONS_ACK() { memset(this, 0, sizeof(tagPKT_GSPK_RET_FACTIONS_ACK)); }
	DT_PK_FACTION_LIST stRetFactions; //门派信息列表
}PKT_GSPK_RET_FACTIONS_ACK;

typedef struct tagPKT_PKGS_GET_PLAYER_REQ{
	tagPKT_PKGS_GET_PLAYER_REQ() { memset(this, 0, sizeof(tagPKT_PKGS_GET_PLAYER_REQ)); }
	UINT32           dwPKGroupID; //所处的战斗组ID
	UINT8            byGetCnt; //调取的玩家数量
	UINT32           adwPlayerIDList[MAX_REQ_SYNC_PLAYER_CNT_ONCE]; //调取的玩家ID列表
}PKT_PKGS_GET_PLAYER_REQ;

typedef struct tagPKT_GSPK_RET_PLAYER_ACK{
	tagPKT_GSPK_RET_PLAYER_ACK() { memset(this, 0, sizeof(tagPKT_GSPK_RET_PLAYER_ACK)); }
	UINT32           dwPKGroupID; //所处的战斗组ID
	UINT8            byRetCnt; //返回的玩家数量
	DT_SYNC_PLAYER   astPlayerList[MAX_RET_SYNC_PLAYER_CNT_ONCE]; //调取的玩家ID列表
}PKT_GSPK_RET_PLAYER_ACK;

typedef struct tagPKT_GSPK_GET_VIDEO_REQ{
	tagPKT_GSPK_GET_VIDEO_REQ() { memset(this, 0, sizeof(tagPKT_GSPK_GET_VIDEO_REQ)); }
	UINT8            byEmpty; //无用字段
}PKT_GSPK_GET_VIDEO_REQ;

typedef struct tagPKT_PKGS_RET_VIDEO_ACK{
	tagPKT_PKGS_RET_VIDEO_ACK() { memset(this, 0, sizeof(tagPKT_PKGS_RET_VIDEO_ACK)); }
	UINT64           qwVideoID; //返回的录像ID
	UINT8            byStartSlot; //返回的录像列表中第一个录像的位置
	UINT16           wPvPVideoCnt; //返回该录像中的玩家录像数
	DT_GVG_PK_VIDEO  astPvPVideoList[GVG_PK_VIDEO_RET_CNT_ONCE]; //返回的玩家录像列表
}PKT_PKGS_RET_VIDEO_ACK;

typedef struct tagPKT_PKGS_SYNC_VIDEO_NTF{
	tagPKT_PKGS_SYNC_VIDEO_NTF() { memset(this, 0, sizeof(tagPKT_PKGS_SYNC_VIDEO_NTF)); }
	UINT16           wVideoCnt; //录像个数
	DT_GVG_VIDEO     astVideoList[GVG_VIDEO_SYNC_CNT_ONCE]; //录像列表
}PKT_PKGS_SYNC_VIDEO_NTF;

typedef struct tagPKT_PKGS_SYNC_GVG_NTF{
	tagPKT_PKGS_SYNC_GVG_NTF() { memset(this, 0, sizeof(tagPKT_PKGS_SYNC_GVG_NTF)); }
	DT_GVG_LOCAL_ACTIVITY_DATA stGvGInfo; //该区的跨服战信息
}PKT_PKGS_SYNC_GVG_NTF;

typedef struct tagDT_FACTION_SUPPORT_INFO{
	UINT32           dwFactionID; //门派ID
	UINT32           dwSupportCnt; //支持数
}DT_FACTION_SUPPORT_INFO;

typedef struct tagDT_FACTION_SUPPORT_LIST{
	UINT32           dwFactionCnt; //门派数
	DT_FACTION_SUPPORT_INFO astSupportList[MAX_GVG_ARENA_FACTION_CNT]; //各门派的支持数信息
}DT_FACTION_SUPPORT_LIST;

typedef struct tagPKT_GSPK_SUPPORT_FACTION_REQ{
	tagPKT_GSPK_SUPPORT_FACTION_REQ() { memset(this, 0, sizeof(tagPKT_GSPK_SUPPORT_FACTION_REQ)); }
	UINT32           dwArenaID; //赛区ID
	UINT8            byBigRound; //轮次
	DT_FACTION_SUPPORT_LIST stFactionSupportInfo; //门派数
}PKT_GSPK_SUPPORT_FACTION_REQ;

typedef struct tagPKT_PKGS_SYNC_FACTION_SUPPORT_NTF{
	tagPKT_PKGS_SYNC_FACTION_SUPPORT_NTF() { memset(this, 0, sizeof(tagPKT_PKGS_SYNC_FACTION_SUPPORT_NTF)); }
	DT_FACTION_SUPPORT_LIST stFactionSupportInfo; //门派数
}PKT_PKGS_SYNC_FACTION_SUPPORT_NTF;

typedef struct tagPKT_PKGS_GVG_ASK_READY_REQ{
	tagPKT_PKGS_GVG_ASK_READY_REQ() { memset(this, 0, sizeof(tagPKT_PKGS_GVG_ASK_READY_REQ)); }
	UINT8            byStage; //跨服门派战当前阶段
}PKT_PKGS_GVG_ASK_READY_REQ;

typedef struct tagPKT_GSPK_GVG_REPLY_READY_ACK{
	tagPKT_GSPK_GVG_REPLY_READY_ACK() { memset(this, 0, sizeof(tagPKT_GSPK_GVG_REPLY_READY_ACK)); }
	UINT8            byStage; //跨服门派战当前阶段
	UINT8            byIsReady; //是否已就绪
}PKT_GSPK_GVG_REPLY_READY_ACK;

typedef struct tagPKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ{
	tagPKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ() { memset(this, 0, sizeof(tagPKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ)); }
	UINT8            byNothing; //无效数据
}PKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ;

typedef struct tagPKT_PKGS_GVG_ASK_OPEN_REQ{
	tagPKT_PKGS_GVG_ASK_OPEN_REQ() { memset(this, 0, sizeof(tagPKT_PKGS_GVG_ASK_OPEN_REQ)); }
	UINT8            byNothing; //无效数据
}PKT_PKGS_GVG_ASK_OPEN_REQ;

typedef struct tagPKT_GSPK_GVG_REPLY_OPEN_ACK{
	tagPKT_GSPK_GVG_REPLY_OPEN_ACK() { memset(this, 0, sizeof(tagPKT_GSPK_GVG_REPLY_OPEN_ACK)); }
	UINT8            byIsCanOpen; //是否可开启：1是/0否
}PKT_GSPK_GVG_REPLY_OPEN_ACK;

/*Define Structs and Unions        End*/


enum EN_ProtoGSPK_MessageID
{
	PKGS_GVG_GET_TOP_8_REQ          = 3100        ,
	GSPK_GVG_RET_TOP_8_ACK          = 3101        ,
	PKGS_GET_FACTIONS_REQ           = 3102        ,
	GSPK_RET_FACTIONS_ACK           = 3103        ,
	PKGS_GET_PLAYER_REQ             = 3104        ,
	GSPK_RET_PLAYER_ACK             = 3105        ,
	PKGS_GVG_NEXT_ROUND_NTF         = 3106        ,
	PKGS_SYNC_GVG_NTF               = 3108        ,
	GSPK_GET_VIDEO_REQ              = 3109        ,
	PKGS_RET_VIDEO_ACK              = 3110        ,
	PKGS_SYNC_VIDEO_NTF             = 3111        ,
	GSPK_SUPPORT_FACTION_REQ        = 3112        ,
	PKGS_SYNC_FACTION_SUPPORT_NTF   = 3113        ,
	PKGS_GVG_ASK_READY_REQ          = 3114        ,
	GSPK_GVG_REPLY_READY_ACK        = 3115        ,
	GSPK_GET_GVG_ACTIVITY_DATA_REQ  = 3116        ,
	PKGS_GVG_ASK_OPEN_REQ           = 3117        ,
	GSPK_GVG_REPLY_OPEN_ACK         = 3118        ,
};

/*Encode and Decode Function Declare Begin*/
INT32 EncodeDT_SYNC_PLAYER(void *pHost, CNetData* poNetData);
INT32 DecodeDT_SYNC_PLAYER(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_GVG_NEXT_ROUND_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_GVG_NEXT_ROUND_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_GVG_GET_TOP_8_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_GVG_GET_TOP_8_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSPK_GVG_RET_TOP_8_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSPK_GVG_RET_TOP_8_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_GET_FACTIONS_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_GET_FACTIONS_REQ(void *pHost, CNetData* poNetData);
INT32 EncodeDT_PK_FACTION_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_PK_FACTION_LIST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSPK_RET_FACTIONS_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSPK_RET_FACTIONS_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_GET_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_GET_PLAYER_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSPK_RET_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSPK_RET_PLAYER_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSPK_GET_VIDEO_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSPK_GET_VIDEO_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_RET_VIDEO_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_RET_VIDEO_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_SYNC_VIDEO_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_SYNC_VIDEO_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_SYNC_GVG_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_SYNC_GVG_NTF(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_SUPPORT_INFO(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_SUPPORT_INFO(void *pHost, CNetData* poNetData);
INT32 EncodeDT_FACTION_SUPPORT_LIST(void *pHost, CNetData* poNetData);
INT32 DecodeDT_FACTION_SUPPORT_LIST(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSPK_SUPPORT_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSPK_SUPPORT_FACTION_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_SYNC_FACTION_SUPPORT_NTF(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_SYNC_FACTION_SUPPORT_NTF(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_GVG_ASK_READY_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_GVG_ASK_READY_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSPK_GVG_REPLY_READY_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSPK_GVG_REPLY_READY_ACK(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSPK_GET_GVG_ACTIVITY_DATA_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_PKGS_GVG_ASK_OPEN_REQ(void *pHost, CNetData* poNetData);
INT32 DecodePKT_PKGS_GVG_ASK_OPEN_REQ(void *pHost, CNetData* poNetData);
INT32 EncodePKT_GSPK_GVG_REPLY_OPEN_ACK(void *pHost, CNetData* poNetData);
INT32 DecodePKT_GSPK_GVG_REPLY_OPEN_ACK(void *pHost, CNetData* poNetData);
/*Encode and Decode Function Declare End*/

typedef INT32 (*EnFuncCProtoGSPK)(void *pHost, CNetData* poNetData);
typedef INT32 (*DeFuncCProtoGSPK)(void *pHost, CNetData* poNetData);

class CProtoGSPK : public CSDProtocol
{
public:
    static CProtoGSPK * Instance();
    virtual INT32 Encode(INT32 iMsgID, void *pHost, CHAR *pNet, INT32 iNetSize);
    virtual INT32 Decode(INT32 iMsgID, CHAR *pNet,INT32 iNetSize, void *pHost, INT32 iHostSize);
	INT32 GetMaxPkgSize(){ return m_iMaxPkgSize; }
	EnFuncCProtoGSPK FindEncodeFunc(INT32 iMsgID);
	DeFuncCProtoGSPK FindDecodeFunc(INT32 iMsgID);
	BOOL Init();

private:
	CProtoGSPK();
	virtual ~CProtoGSPK();
	EnFuncCProtoGSPK m_EncodeFuncArray[PROTOGSPK_MSGID_RANGE];
	DeFuncCProtoGSPK m_DecodeFuncArray[PROTOGSPK_MSGID_RANGE];
	CNetData m_oData;
	INT32 m_iMaxPkgSize;

};

#pragma pack(pop)
#endif
