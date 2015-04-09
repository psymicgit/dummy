#include "dealframechangename72.h"
#include "logic/http/namehttpcmd.h"
#include "logic/other/state.h"
#include "logic/player/player.h"
#include "logic/player/playermgr.h"
#include "net/recharge/rcclient.h"
CDealFrameChangeName::CDealFrameChangeName()
{

}

CDealFrameChangeName::~CDealFrameChangeName()
{

}

UINT16 CDealFrameChangeName::GetFrameType()
{
	return EGMT_CHAGNE_NAME;
}

string CDealFrameChangeName::GetStrFrameType()
{
	return CHANGE_PLAYER_NAME;
}

string CDealFrameChangeName::GetStrFrameDesc()
{
	return SDGBK2UTF8(CHANGE_PLAYER_NAME_DESC);
}

void CDealFrameChangeName::Interface(CRCClient* poClinet)
{
	if (NULL == poClinet)
	{
		return;
	}

	map<string,string> mapField;
	mapField[PLAYER_ID] = SDGBK2UTF8(PLAYER_USER_ID)+ "|" + GM_PARAM_PLAYER_ID;
	mapField[CHANGE_PLAYER_NAME] = SDGBK2UTF8(PLAYER_NAME) + "|" + GM_PARAM_STR;

	RsponeMsg(poClinet,mapField);
}

void CDealFrameChangeName::Deal(CRCClient* poClinet,UINT32 dwSerialNumber,UINT32 dwAreaID,UINT32 dwPlayerID,map<string,string> &mapField)
{
	if(NULL == poClinet)
	{
		return;
	}

	if (mapField[PLAYER_ID].empty()
		|| mapField[CHANGE_PLAYER_NAME].empty())
	{
		string strErrorMsg = GetRsponeResult(ERR_GM_PARM_INFO::ID_PARAM_ERR);
		poClinet->Rspone(strErrorMsg.c_str());
	}

	UINT32 unPlayerId = SDAtou(mapField[PLAYER_ID].c_str());
	string strPlayerName = mapField[CHANGE_PLAYER_NAME];

	Deal(poClinet, dwSerialNumber, 0, dwAreaID, dwPlayerID, 0, 0, 0, (TCHAR*)strPlayerName.c_str(), NULL, NULL);
}

void CDealFrameChangeName::Deal(CRCClient* poClinet, UINT32 dwSerialNumber, UINT8 byAction, UINT32 dwAreaID, UINT32 dwPlayerID, UINT32 dwParam1, UINT64 qwParam2, UINT64 qwParam3, TCHAR* pszParam4, TCHAR* pszParam5 , TCHAR *pszDesc)
{
	if((NULL == poClinet) || (NULL == pszParam4))
	{
		return;
	}

	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
	if(NULL == poPlayer)
	{
		poClinet->Rspone("{\"status\":\"Player no in mem!\"}");
		return;
	}

	//检查各个参数是否有sql嵌入攻击
	if(HaveSqlToken(pszParam4))
	{
		poClinet->Rspone("{\"status\":\"sql failed\"}");
		return;
	}

	//检测是否有特殊字符，且长度合法(昵称允许6个中文字符，12个英文)
	if(CkHaveSpeCharacterAndValidLen(pszParam4))
	{
		poClinet->Rspone("{\"status\":\"Special Char,or string too long failed\"}");
		return;
	}

	if( SUCCESS == CPlayerBaseDataCacheMgr::Instance()->RegDspName(dwPlayerID, pszParam4))
	{
		poClinet->Rspone("{\"status\":\"success\"}");

		SNameInfo stNameInfo;
		stNameInfo.dwPlayerID = dwPlayerID;
		stNameInfo.strName    = pszParam4;
		stNameInfo.strOldName = poPlayer->GetDT_PLAYER_BASE_DATA().aszDispName;
		stNameInfo.byAuthType = poPlayer->GetAuthType();
		poPlayer->ChangeNameByGM(pszParam4);

		poPlayer->GetState().GetDT_STATE_DATA().qwRegisterDispNameTime = SDTimeSecs();

		CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_NAME);
		if (NULL == poHttpCmd)
		{
			SYS_CRITICAL( _SDT("[%s:%d] player:%u in memory"), MSG_MARK, dwPlayerID );
			return ;
		}

		poHttpCmd->SetUserData(&stNameInfo, sizeof(SNameInfo), 0);
		poHttpCmd->Init();
		if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
		{
			CHttpMgr::Instance()->FreeCmd(poHttpCmd);
			SYS_CRITICAL( _SDT("[%s:%d] can't AddCommand player:%u in memory"), MSG_MARK, stNameInfo.dwPlayerID );
			return ;
		}
		return;
	}
	else
	{
		poClinet->Rspone("{\"status\":\"failed\"}");
		return;
	}
	
}

void CDealFrameChangeName::Release()
{

}