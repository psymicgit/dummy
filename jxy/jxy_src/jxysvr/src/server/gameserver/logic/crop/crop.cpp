
#include "crop.h"
#include <common/client/errdef.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include "cropseedpropmgr.h"
#include "logic/item/itemmgr.h"
#include "cropfieldpropmgr.h"
#include "logic/task/taskmgr.h"
#include "logic/other/singleprammgr.h"
using namespace SGDP;


IMPLEMENT_CLASS_POOL_MGR(CCrop)

CCrop::CCrop()
{
    memset(&m_stDT_CROP_DATA, 0, sizeof(m_stDT_CROP_DATA));
}


CCrop::~CCrop()
{

}

BOOL CCrop::Init(DT_CROP_DATA &stDT_CROP_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;
    memcpy(&m_stDT_CROP_DATA, &stDT_CROP_DATA, sizeof(DT_CROP_DATA));
    return TRUE;
}


//获取种子信息
UINT16 CCrop::GetDT_CROP_SEED_INFO_DATA(DT_CROP_SEED_INFO_DATA stDT_CROP_SEED_INFO_DATA[MAX_CROP_SEED_NUM], UINT8 &byCropSeedNum)
{
	byCropSeedNum = 0;
	DT_CROP_SEED_INFO_DATA v_stDT_CROP_SEED_INFO_DATA[MAX_CROP_SEED_NUM];
	memset(stDT_CROP_SEED_INFO_DATA, 0x00, sizeof(DT_CROP_SEED_INFO_DATA) * MAX_CROP_SEED_NUM);
	memset(v_stDT_CROP_SEED_INFO_DATA, 0x00, sizeof(DT_CROP_SEED_INFO_DATA) * MAX_CROP_SEED_NUM);


	CCropPropMapItr iter;
	CCropPropMap & mapCropProp = CCropSeedPropMgr::Instance()->GetCropPropMap();
	for (iter = mapCropProp.begin(); iter != mapCropProp.end(); iter++)
	{
		SCropProp &stCropProp = iter->second;
		CGoods * poGood = m_poOwner->GetBag().GetGoodsItem(stCropProp.wCropKindID);
		if (NULL == poGood)
		{
			v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].wCropSeedNum = 0;
		}
		else
		{
			v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].wCropSeedNum  =	poGood->GetCurPileNum();
		}

		SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stCropProp.wCropKindID);
		if (NULL  == pstGoodsProp)
		{
			v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].wComposeCount = 0;
			v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].wComposeKindID = 0;
		}
		else
		{
			v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].wComposeCount = pstGoodsProp->wComposeCount;
			v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].wComposeKindID = pstGoodsProp->wComposeKindID;
		}
		switch(CItemMgr::Instance()->GetItemKindBYKindID(stCropProp.wCropKindID))
		{
		case EIK_EQUIP:
			{
				SEquipProp* pstProp = CEquipPropMgr::Instance()->GetProp(stCropProp.wCropKindID);
				if(NULL == pstProp)
				{
					continue;
				}
				v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].dwGold = pstProp->dwBuyGold;
				v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].dwCoin = pstProp->dwBuyCoin;
			}
			break;
		case EIK_GOODS:
			{
				SGoodsProp* pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stCropProp.wCropKindID);
				if(NULL == pstProp)
				{
					continue;
				}
				v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].dwGold = pstProp->dwBuyGold;
				v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].dwCoin = pstProp->dwBuyCoin;
			}
			break;
		default:
			v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].dwGold = 0;
			v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].dwCoin = 0;
			break;
		}

		v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].wCropKindID	=	stCropProp.wCropKindID;
		v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].byCropType	=	stCropProp.byType;
		v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].dwCropTime	=	stCropProp.wObtainCD;
		v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].dwCropValue	=	stCropProp.dwObtainValue;
		v_stDT_CROP_SEED_INFO_DATA[byCropSeedNum].wCropGainKindID	= stCropProp.wObtainKindID;

		byCropSeedNum++;
		if (byCropSeedNum >= MAX_CROP_SEED_NUM)
		{
			break;
		}
	}

	INT32	index = 0;
	for(INT32 i = 0; i < byCropSeedNum; i++)
	{
		//铜钱种子
		if (ECPK_COIN == v_stDT_CROP_SEED_INFO_DATA[i].byCropType)
		{
			memcpy(&stDT_CROP_SEED_INFO_DATA[index], &v_stDT_CROP_SEED_INFO_DATA[i], sizeof(DT_CROP_SEED_INFO_DATA));
			index++;
		}
	}

	for(INT32 i = 0; i < byCropSeedNum; i++)
	{
		//非铜钱种子
		if (ECPK_COIN != v_stDT_CROP_SEED_INFO_DATA[i].byCropType)
		{
			memcpy(&stDT_CROP_SEED_INFO_DATA[index], &v_stDT_CROP_SEED_INFO_DATA[i], sizeof(DT_CROP_SEED_INFO_DATA));
			index++;
		}
	}

	return ERR_OPEN_CROP::ID_SUCCESS;
}




UINT16 CCrop::GetDT_CROP_FIELD_INFO_DATA(DT_CROP_FIELD_INFO_DATA stDT_CROP_FIELD_INFO_DATA[MAX_CROP_NUM]) //田地信息
{

	CCropFieldPropMapItr itr;
	memset(stDT_CROP_FIELD_INFO_DATA, 0x00, sizeof(DT_CROP_FIELD_INFO_DATA) * MAX_CROP_NUM);
	CCropFieldPropMap &mapCCropFieldProp = CCropFieldPropMgr::Instance()->GetCropFieldPropMap();
	for (int i = 0; i < MAX_CROP_NUM; i++)
	{
		UINT8 byFieldID = i + 1;
		stDT_CROP_FIELD_INFO_DATA[i].byOpen  = m_stDT_CROP_DATA.astCropInfoList[i].byOpen;

		itr = mapCCropFieldProp.find(byFieldID);
		if (itr != mapCCropFieldProp.end())
		{
			stDT_CROP_FIELD_INFO_DATA[i].byLevel = itr->second.byLevel;
			stDT_CROP_FIELD_INFO_DATA[i].dwGold = itr->second.dwGold;
			stDT_CROP_FIELD_INFO_DATA[i].dwCoin = itr->second.dwCoin;
		}
		else
		{
			stDT_CROP_FIELD_INFO_DATA[i].byLevel = 0;
			stDT_CROP_FIELD_INFO_DATA[i].dwGold = 0;
			stDT_CROP_FIELD_INFO_DATA[i].dwCoin = 0;
		}
	}
	return ERR_OPEN_CROP::ID_SUCCESS;
}

UINT16	CCrop::GetDT_CROP_FIELD_INFO_DATA(UINT8 byFieldID, DT_CROP_FIELD_INFO_DATA &stCropFieldInfo) //田地信息
{

	//种植未开放，超过索引、ID不存在
	if (byFieldID > MAX_CROP_NUM || byFieldID < 1)
	{
		return ERR_CROP::ID_CROP_IS_NOT_OPEN;
	}

	memset(&stCropFieldInfo, 0x00, sizeof(DT_CROP_FIELD_INFO_DATA));
	CCropFieldPropMapItr itr;
	CCropFieldPropMap &mapCCropFieldProp = CCropFieldPropMgr::Instance()->GetCropFieldPropMap();
	stCropFieldInfo.byOpen  = m_stDT_CROP_DATA.astCropInfoList[byFieldID - 1].byOpen;

	itr = mapCCropFieldProp.find(byFieldID);
	if (itr != mapCCropFieldProp.end())
	{
		stCropFieldInfo.byLevel = itr->second.byLevel;
		stCropFieldInfo.dwGold = itr->second.dwGold;
		stCropFieldInfo.dwCoin = itr->second.dwCoin;
	}
	return ERR_OPEN_CROP::ID_SUCCESS;
}

UINT16 CCrop::GetDT_CROP_MORE_INFO_DATA(DT_CROP_MORE_INFO_DATA stDT_CROP_MORE_INFO_DATA[MAX_CROP_NUM])
{

	SCropProp * pCropProp = NULL;
	memset(stDT_CROP_MORE_INFO_DATA, 0x00, sizeof(DT_CROP_MORE_INFO_DATA) * MAX_CROP_NUM);
	DT_CROP_DATA &stDT_CROP_DATA = m_stDT_CROP_DATA;
	for (int i = 0; i < MAX_CROP_NUM; i++)
	{
		if (stDT_CROP_DATA.astCropInfoList[i].byOpen &&
			stDT_CROP_DATA.astCropInfoList[i].wCropKindID > 0)
		{
			pCropProp = CCropSeedPropMgr::Instance()->GetCropProp(m_stDT_CROP_DATA.astCropInfoList[i].wCropKindID);
			if (NULL == pCropProp)
			{
				continue;
			}

			//计算经过的时间
			UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[i].qwLastCropTime;
			stDT_CROP_MORE_INFO_DATA[i].wCropKindID		=	pCropProp->wCropKindID;
			stDT_CROP_MORE_INFO_DATA[i].dwCropTime		=	(UINT32)(pCropProp->wObtainCD > qwDiffSecond ? (pCropProp->wObtainCD - qwDiffSecond) : 0);
			stDT_CROP_MORE_INFO_DATA[i].byCropType		=	pCropProp->byType;
			stDT_CROP_MORE_INFO_DATA[i].dwCropValue		=	pCropProp->dwObtainValue;
			stDT_CROP_MORE_INFO_DATA[i].wCropGainKindID	=	pCropProp->wObtainKindID;
		}
	}
	return ERR_OPEN_CROP::ID_SUCCESS;
}

UINT16	 CCrop::GetDT_CROP_MORE_INFO_DATA(UINT8 byFieldID,  DT_CROP_MORE_INFO_DATA &stDT_CROP_MORE_INFO_DATA)
{
	//种植未开放，超过索引、ID不存在
	if (byFieldID > MAX_CROP_NUM || byFieldID < 1)
	{
		return ERR_CROP::ID_CROP_IS_NOT_OPEN; ;
	}
	byFieldID--;
	SCropProp * pCropProp = NULL;
	memset(&stDT_CROP_MORE_INFO_DATA, 0x00, sizeof(DT_CROP_MORE_INFO_DATA));

	DT_CROP_DATA &stDT_CROP_DATA = m_stDT_CROP_DATA;
	if (stDT_CROP_DATA.astCropInfoList[byFieldID].byOpen &&
		stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID > 0)
	{
		pCropProp = CCropSeedPropMgr::Instance()->GetCropProp(m_stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID);
		if (NULL == pCropProp)
		{
			return ERR_CROP::ID_FIELD_NOT_EXIT;;
		}

		//计算经过的时间
		UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime;
		stDT_CROP_MORE_INFO_DATA.wCropKindID	=	pCropProp->wCropKindID;
		stDT_CROP_MORE_INFO_DATA.dwCropTime		=	(UINT32)(pCropProp->wObtainCD > qwDiffSecond ? (pCropProp->wObtainCD - qwDiffSecond) : 0);
		stDT_CROP_MORE_INFO_DATA.byCropType		=	pCropProp->byType;
		stDT_CROP_MORE_INFO_DATA.dwCropValue	=	pCropProp->dwObtainValue;
		stDT_CROP_MORE_INFO_DATA.wCropGainKindID	=	pCropProp->wObtainKindID;
	}
	return ERR_CROP::ID_SUCCESS;
}



UINT16	CCrop::Crop(UINT8 byFieldID, UINT16 wCropKindID)
{
	//超过索引、ID不存在
	if (byFieldID > MAX_CROP_NUM || byFieldID < 1)
	{
		return ERR_CROP::ID_FIELD_NOT_EXIT;
	}

	SCropFieldProp *pCropFieldProp = CCropFieldPropMgr::Instance()->GetCropFieldProp(byFieldID);
	if (NULL == pCropFieldProp)
	{
		return ERR_CROP::ID_FIELD_NOT_EXIT;
	}

	byFieldID--;
	DT_CROP_DATA &stDT_CROP_DATA = m_stDT_CROP_DATA;
	//地块未开放
	if(0 == stDT_CROP_DATA.astCropInfoList[byFieldID].byOpen)
	{
		return ERR_CROP::ID_FIELD_NOT_OPEN;
	}

	//地块已种植
	if(stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID)
	{
		return ERR_CROP::ID_FIELD_HAVE_CROP;
	}

	//检测种子是否存在
	CGoods * poGoods = m_poOwner->GetBag().GetGoodsItem(wCropKindID);
	if (NULL == poGoods)
	{
		return ERR_CROP::ID_SEED_NOT_EXIT;
	}
	if(poGoods->GetCurPileNum() <= 0)
	{
		return ERR_CROP::ID_SEED_NOT_EXIT;
	}

	//扣除
	//   poGoods->GetDT_GOODS_DATA().wPileCount--;

	////日志
	//   CRecordMgr::Instance()->RecordInfo( GetID(), ERM_DECITEM, CRecordMgr::EDIT_CROP, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetCurPileNum(),
	//	GetLevel(), m_stDT_PLAYER_BASE_DATA.byVipLevel, 1);

	//扣除操作
	poGoods->DecPileNum(1, CRecordMgr::EDIT_CROP);

	if(0 == poGoods->GetCurPileNum())
	{
		m_poOwner->OnDeleteItem(poGoods);
	}

	stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID = wCropKindID;
	stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime = SDTimeSecs();
	CTaskMgr::Instance()->OnCrop(m_poOwner);
	return ERR_CROP::ID_SUCCESS;
}


UINT16	CCrop::GainCrop(UINT8 byFieldID, UINT8 &byCropType, UINT32 &dwCropValue, UINT16 &wCropKindID)
{
	//超过索引、ID不存在
	if (byFieldID > MAX_CROP_NUM || byFieldID < 1)
	{
		return ERR_GAIN_CROP::ID_FIELD_NOT_EXIT;
	}

	SCropFieldProp *pCropFieldProp = CCropFieldPropMgr::Instance()->GetCropFieldProp(byFieldID);
	if (NULL == pCropFieldProp)
	{
		return ERR_GAIN_CROP::ID_FIELD_NOT_EXIT;
	}
	byFieldID--;


	DT_CROP_DATA &stDT_CROP_DATA = m_stDT_CROP_DATA;
	//未引导
	if(m_poOwner->CKGuide( EGF_CROP))
	{
		CCropPropMapItr iter;
		CCropPropMap & mapCropProp = CCropSeedPropMgr::Instance()->GetCropPropMap();
		for (iter = mapCropProp.begin(); iter != mapCropProp.end(); iter++)
		{
			//引导只会是经验种子
			SCropProp &stCropProp = iter->second;
			if (ECPK_COIN != stCropProp.byType)
			{
				continue;
			}

			CGoods * poGood = m_poOwner->GetBag().GetGoodsItem(stCropProp.wCropKindID);
			if (NULL != poGood)
			{
				stDT_CROP_DATA.astCropInfoList[byFieldID].byOpen = 1;
				stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime = 0;
				stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID = stCropProp.wCropKindID;
				break;
			}
		}

		//如果背包中没有种子的话，随便种一颗铜钱种子
		if (0 == stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID)
		{
			for (iter = mapCropProp.begin(); iter != mapCropProp.end(); iter++)
			{
				//引导只会是经验种子
				SCropProp &stCropProp = iter->second;
				if (ECPK_COIN == stCropProp.byType)
				{
					stDT_CROP_DATA.astCropInfoList[byFieldID].byOpen = 1;
					stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime = 0;
					stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID = stCropProp.wCropKindID;
					break;
				}
			}
		}

		//检测种子是否存在
		CGoods * poGoods = NULL;
		poGoods = m_poOwner->GetBag().GetGoodsItem(stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID);
		if (NULL != poGoods)
		{
			if(0 != poGoods->GetCurPileNum())
			{
				////扣除
				//poGoods->GetDT_GOODS_DATA().wPileCount--;
				////引导多给了一个种子，需要扣除
				//CRecordMgr::Instance()->RecordInfo( GetID(), ERM_DECITEM, CRecordMgr::EDIT_CROP, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetCurPileNum(),
				//GetLevel(), m_stDT_PLAYER_BASE_DATA.byVipLevel, 1);

				//扣除操作
				poGoods->DecPileNum(1, CRecordMgr::EDIT_CROP);
			}
			if(0 == poGoods->GetDT_GOODS_DATA().wPileCount)
			{
				m_poOwner->OnDeleteItem(poGoods);
			}
		}
	}


	CTaskMgr::Instance()->OnCrop(m_poOwner);
	//增加任务指引
	m_poOwner->AddGuideRecord( EGF_CROP);

	//地块未开放
	if(0 == stDT_CROP_DATA.astCropInfoList[byFieldID].byOpen)
	{
		return ERR_GAIN_CROP::ID_FIELD_NOT_OPEN;
	}

	//地块已种植
	if(0 == stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID)
	{
		return ERR_GAIN_CROP::ID_FIELD_NOT_CROP;
	}

	//检测时间是否达到收获
	SCropProp *  poCropProp = NULL;
	poCropProp = CCropSeedPropMgr::Instance()->GetCropProp(stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID);
	if (NULL == poCropProp)
	{
		//出错异常处理
		stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID = 0;
		stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime = 0;
		return ERR_GAIN_CROP::ID_FIELD_NOT_CROP;
	}
	UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime;
	if(poCropProp->wObtainCD > qwDiffSecond)
	{
		return ERR_GAIN_CROP::ID_NOT_TIME_OUT;
	}

	wCropKindID = 0;
	byCropType = poCropProp->byType;
	dwCropValue = poCropProp->dwObtainValue;
	switch(poCropProp->byType)
	{
	case ECPK_COIN:
		{
			m_poOwner->AddCoin(poCropProp->dwObtainValue, CRecordMgr::EACT_CROP, poCropProp->wCropKindID, poCropProp->byType);
		}
		break;
	case ECPK_SCIENCE:
		{
			m_poOwner->AddScience(poCropProp->dwObtainValue, CRecordMgr::EAST_CROP, poCropProp->wCropKindID, poCropProp->byType);
		}
		break;
	case ECPK_EXPRIENCE:
		{
			ECreateItemRet	emCreateItemRet  = ECIR_SUCCESS;
			CItem	*		poItem		   = NULL;
			wCropKindID = poCropProp->wObtainKindID;
			//poItem = CItemMgr::Instance()->CreateGoodsIntoBag(this, poCropProp->wObtainKindID, poCropProp->dwObtainValue, emCreateItemRet, CRecordMgr::EAIT_CROP);
			poItem =  CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, poCropProp->wObtainKindID, poCropProp->dwObtainValue, emCreateItemRet, 0, CRecordMgr::EAIT_CROP);

			//背包满
			if(NULL == poItem && ECIR_FAILED_BAG_FULL == emCreateItemRet)
			{
				return ERR_GAIN_CROP::ID_BAG_FULL;
			}
			else if (NULL == poItem )
			{
				return ERR_GAIN_CROP::ID_FIELD_NOT_CROP;
			}
		}
		break;
	case ECPK_STORY:
		{
			//GetDT_PLAYER_BASE_DATA().qwStory += poCropProp->dwObtainValue;
			m_poOwner->AddStory(poCropProp->dwObtainValue, CRecordMgr::EASTT_CROP, poCropProp->wCropKindID, poCropProp->byType);
		}
		break;
	case ECPK_JINGJIE:
		{
			m_poOwner->AddJingJie(poCropProp->dwObtainValue, CRecordMgr::EASTT_CROP, poCropProp->wCropKindID, poCropProp->byType);
		}
		break;
	default:
		break;
	}

	stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID = 0;
	stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime = 0;
	return ERR_GAIN_CROP::ID_SUCCESS;
}

UINT16	CCrop::OpenCropField(UINT8 byFieldID)
{
	//超过索引、ID不存在
	if (byFieldID > MAX_CROP_NUM || byFieldID < 1)
	{
		return ERR_CROP_FIELD::ID_FIELD_NOT_EXIT;
	}

	SCropFieldProp *pCropFieldProp = CCropFieldPropMgr::Instance()->GetCropFieldProp(byFieldID);
	if (NULL == pCropFieldProp)
	{
		return ERR_CROP_FIELD::ID_FIELD_NOT_EXIT;
	}
	byFieldID--;
	DT_CROP_DATA &stDT_CROP_DATA = m_stDT_CROP_DATA;
	//地块已开放
	if(stDT_CROP_DATA.astCropInfoList[byFieldID].byOpen)
	{
		return ERR_CROP_FIELD::ID_FIELD_IS_OPEN;
	}

	if(m_poOwner->GetLevel() < pCropFieldProp->byLevel)
	{
		return ERR_CROP_FIELD::ID_LEVEL_NOT_ENOUGH;
	}
	if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwCoin < pCropFieldProp->dwCoin)
	{
		return ERR_COMMON::ID_COIN_NOT_ENOUGH;
	}
	if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < pCropFieldProp->dwGold)
	{
		return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
	}
	//GetDT_PLAYER_BASE_DATA().qwCoin -= pCropFieldProp->dwCoin;
	//GetDT_PLAYER_BASE_DATA().qwGold -= pCropFieldProp->dwGold;
	if(0 != pCropFieldProp->dwCoin)
	{
		m_poOwner->DecCoin(pCropFieldProp->dwCoin, CRecordMgr::EDCT_CROP, pCropFieldProp->wKindID );
	}
	if(0 != pCropFieldProp->dwGold)
	{
		m_poOwner->DecGold(pCropFieldProp->dwGold, CRecordMgr::EDGT_OPENCROPFIELD, pCropFieldProp->wKindID );
	}

	stDT_CROP_DATA.astCropInfoList[byFieldID].byOpen = 1;
	return ERR_CROP_FIELD::ID_SUCCESS;
}

UINT16	CCrop::ClrCrop(UINT8 byFieldID)
{
	//超过索引、ID不存在
	if (byFieldID > MAX_CROP_NUM || byFieldID < 1)
	{
		return ERR_CLR_CROP::ID_FIELD_NOT_EXIT;
	}

	SCropFieldProp *pCropFieldProp = CCropFieldPropMgr::Instance()->GetCropFieldProp(byFieldID);
	if (NULL == pCropFieldProp)
	{
		return ERR_CLR_CROP::ID_FIELD_NOT_EXIT;
	}

	byFieldID--;
	DT_CROP_DATA &stDT_CROP_DATA = m_stDT_CROP_DATA;
	//地块未开放
	if(0 == stDT_CROP_DATA.astCropInfoList[byFieldID].byOpen)
	{
		return ERR_CLR_CROP::ID_FIELD_NOT_OPEN;
	}

	//地块已种植
	if(0 == stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID)
	{
		return ERR_CLR_CROP::ID_FIELD_NOT_CROP;
	}

	//检测时间是否达到收获
	SCropProp *  poCropProp = NULL;
	poCropProp = CCropSeedPropMgr::Instance()->GetCropProp(stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID);
	if (NULL == poCropProp)
	{
		//出错异常处理
		stDT_CROP_DATA.astCropInfoList[byFieldID].wCropKindID = 0;
		stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime = 0;
		return ERR_CLR_CROP::ID_FIELD_NOT_CROP;
	}

	UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime;
	if(poCropProp->wObtainCD > qwDiffSecond)
	{
		qwDiffSecond  = poCropProp->wObtainCD - qwDiffSecond;
		UINT64 qwMin = DivCeil(qwDiffSecond, 60);
		UINT32 dwGold = (UINT32)(CSinglePramMgr::Instance()->GetGainCropCDGoldPerMin() * qwMin);
		if (dwGold > m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold)
		{
			return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
		}
		//GetDT_PLAYER_BASE_DATA().qwGold -= dwGold;
		m_poOwner->DecGold(dwGold, CRecordMgr::EDGT_CLRCROP, qwMin);
	}
	stDT_CROP_DATA.astCropInfoList[byFieldID].qwLastCropTime = 0;
	return ERR_CLR_CROP::ID_SUCCESS;
}

