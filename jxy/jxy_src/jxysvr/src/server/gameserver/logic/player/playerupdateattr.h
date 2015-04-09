public:
	INT32 GetUpdateAttrInfo( UINT16 wHeroKindID, UINT8 byAttrKind, DT_UPDATE_ATTR_DATA &data );
	INT32 UpdateAttr( UINT16 wHeroKindID, UINT8 byAttrKind, UINT8 byUpdateKind, UINT16 wAttrUpdateCnt,
		DT_UPDATE_ATTR_DATA &oUpdateAttrData, DT_UPDATED_ATTR_BATTLE_DATA &oBattleData );
	
protected:
	BOOL AddAttrUpdateCnt( CHero *pHero, UINT8 byAttrKind, UINT32 dwPerCountAddValue, UINT16 &wCntResult );
	CGoods *GetUpdateItem( UINT8 byAttr, UINT16 &wItemID );
	DT_UPDATED_ATTR_BATTLE_DATA GetUpdateAttrBattleInfo( CHero *pHero );

	DT_UPDATE_ATTR_DATA GetUpdateAttrData( CHero *pHero, UINT16 wCount, 
		const SUpdateData &inputUpdateData, UINT8 byAttrKind );

	//获得加上升级次数的属性值
	UINT32 GetHeroAttrValue( UINT32 dwPerCountAddValue, UINT16 wCount, UINT8 byAttr, CHero *pHero );