// DealFrame.cpp: implementation of the CRsDealFrame class.
//
//////////////////////////////////////////////////////////////////////



#include "rsdealframe.h"
#include "common/server/ngstring.h"



/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//GM功能
enum EGMERR
{
    EGMERR_ADD_BLUEGAS = 40,	//仙露
    EGMERR_ADD_COIN = 41,	//铜钱
    EGMERR_ADD_EXPERCIENCE = 42,	//经验
    EGMERR_ADD_GOLD = 43,//元宝
    EGMERR_ADD_JINGJIE = 44,//境界
    EGMERR_ADD_PHYSTRENGTH = 45,//体力
    EGMERR_ADD_PURPLEGAS = 46,//功力
    EGMERR_ADD_SCIENCE	=	47,//器魂
    EGMERR_ADD_STORY	=	48,//阅力
    EGMERR_ADD_STUDY	=	49,//卦像
    EGMERR_ADD_STUDYCLI = 50,//乾坤碎片
    EGMERR_ADD_STUDYEXP = 51,//卦像经验
    EGMERR_ADD_STUDYNOATTR = 52,//太极卦像
    EGMERR_ADD_ITEM = 53, //增加物品
    EGMERR_ADD_GOLD1 = 54, //元宝1
    EGMERR_ADD_FACTION_FUNDS = 55, //增加门派资金
    EGMERR_ADD_DOORSTRIBUTE = 56, //增加门派门贡
};

//GM功能
typedef struct
{
    EGMERR emErr;
    char*  strType;
    char*  strMsg;
    UINT32 uMaxValue;
} FrmTab;

//通过验证可以访问
FrmTab frm_tab[] =
{
    { EGMERR_ADD_BLUEGAS,		"bluegas",		"增加仙露超过上限",			1000000},
    { EGMERR_ADD_COIN,		"coin",			"增加铜钱超过上限",			2000000000},
    { EGMERR_ADD_EXPERCIENCE,	"experience",		"增加经验超过上限",			1000000},
    { EGMERR_ADD_GOLD,		"gold",			"增加元宝超过上限",			200000},
    { EGMERR_ADD_JINGJIE,		"jingjie",		"增加境界超过上限",			1000000},
    { EGMERR_ADD_PHYSTRENGTH,	"phystrength",	"增加体力超过上限",			10000},
    { EGMERR_ADD_PURPLEGAS,	"purplegas",		"增加功力超过上限",			1000000},
    { EGMERR_ADD_SCIENCE,		"science",		"增加器魂超过上限",			1000000},
    { EGMERR_ADD_STORY,		"story",			"增加阅力超过上限",			1000000},
    { EGMERR_ADD_STUDY,		"study",			"增加卦像超过上限",			1000000},
    { EGMERR_ADD_STUDYCLI,		"studyclip",		"增加乾坤碎片超过上限",		100},
    { EGMERR_ADD_STUDYEXP,		"studyexp",		"增加卦像经验超过上限",		1000000},
    { EGMERR_ADD_STUDYNOATTR,	"studynoattr",	"增加太极卦像超过上限",		100},
    { EGMERR_ADD_ITEM,		"item",			"增加物品超过上限",			100},
    { EGMERR_ADD_GOLD1,		"gold1",			"增加元宝1超过上限",			100000},
    { EGMERR_ADD_FACTION_FUNDS,"factionfunds",	"增加门派资金超过上限",		100000},
    { EGMERR_ADD_DOORSTRIBUTE,	"doorstribute",	"增加门派门贡超过上限",		100000},
	{ EGMERR_ADD_DOORSTRIBUTE,	"soul",	"增加战魂超过上限",		255},
	{ EGMERR_ADD_DOORSTRIBUTE,	"pet",	"增加宠物超过上限",		250}
};


namespace InterfaceDealBase
{
CRsDealFrame::CRsDealFrame()
{
    m_pManager = NULL;
}

CRsDealFrame::CRsDealFrame(CRsManager * IN_pManager): m_pManager(IN_pManager)
{

}

CRsDealFrame::~CRsDealFrame()
{
    Release();
}

bool CRsDealFrame::init(CRsManager * IN_pManager)
{
    m_pManager = IN_pManager;
    RegisterDealFrame(this);
    return TRUE;
}
void CRsDealFrame::Release()
{

}

//注册处理类型与对象指针的对应关系
void CRsDealFrame::RegisterDealFrame(CRsDealFrame *v_pDealFrame)
{
    if(m_pManager)
        m_pManager->RegisterDealFrame(v_pDealFrame);
}

bool CRsDealFrame::CkAddLimit( string strKey , UINT32 unAddNum)
{
    for (unsigned int i = 0; i < sizeof(frm_tab) / sizeof(FrmTab); i++)
    {
        if (0 == strcmp(frm_tab[i].strType, strKey.c_str()))
        {
            if (frm_tab[i].uMaxValue < unAddNum)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}


CRsManager::CRsManager()
{
    m_pInstance = this;
}


CRsManager::~CRsManager()
{
    Release();
}


//获取本对象实例
CRsManager* CRsManager::instance()
{
    return m_pInstance;
}


void CRsManager::Release()
{
    try
    {
        for(MapTaskIter itr = m_mapTask.begin(); itr != m_mapTask.end(); itr++ )
        {
            itr->second->Release();
            delete itr->second;
        }
        m_mapTask.clear();
    }
    catch(...)
    {
        return ;
    }
    return ;
}


//注册处理类型与对象指针的对应关系
void CRsManager::RegisterDealFrame(CRsDealFrame * IN_pDealFrame)
{
    m_mapTask.insert(std::make_pair(IN_pDealFrame->GetFrameType(), IN_pDealFrame));
}

//获取对象指针
CRsDealFrame * CRsManager::GetInstanceByType(string strKey)
{
    MapTaskIter         iter;
    iter = m_mapTask.find(strKey);
    if( m_mapTask.end() == iter )
    {
        return NULL;
    }
    return iter->second;
}


BOOL CRsManager::Deal( string strKey, CPlayer* poPlayer, vector<string> vecOpValue, UINT8 byOpType, UINT32 dwNum1, UINT32 dwNum2, UINT32 dwNum3, TCHAR *pszDesc, DT_PLAYER_RES_DATA* pstResInfo1, DT_PLAYER_RES_DATA* pstResInfo2)
{
    InterfaceDealBase::CRsDealFrame *pDeal = GetInstanceByType(strKey);
    if(NULL != pDeal)
    {
        return pDeal->Deal(poPlayer, vecOpValue, byOpType, dwNum1, dwNum2, dwNum3, pszDesc, pstResInfo1, pstResInfo2);
    }
    //对于没有查找到的，直接找物品处理接口。
    pDeal = GetInstanceByType("item");
    if (NULL != pDeal)
    {
        return pDeal->Deal(poPlayer, vecOpValue, byOpType, dwNum1, dwNum2, dwNum3, pszDesc, pstResInfo1, pstResInfo2);
    }
    return FALSE;
}

string CRsManager::GetAddLimitMsg( string strKey )
{
    for (unsigned int i = 0; i < sizeof(frm_tab) / sizeof(FrmTab); i++)
    {
        if (0 == strcmp(frm_tab[i].strType, strKey.c_str()))
        {
            CNGString strMsg;
            strMsg += "{\"errno\":";
            strMsg += frm_tab[i].emErr;
            strMsg += ",\"desc\":\"";
            strMsg += SDGBK2UTF8(frm_tab[i].strMsg);
            strMsg += "\"}";

            return strMsg.str();
        }
    }

    return string("");
}

}





