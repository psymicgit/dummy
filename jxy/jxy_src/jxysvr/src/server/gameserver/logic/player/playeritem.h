public:
	VOID GetIdleEquip(DT_EQUIP_DATA_LST_CLI& stEquipList);//获取所有空闲装备
	VOID GetAllEquip(DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI& stEquipList);//获取所有装备
	VOID GetAllEquip(DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI& stEquipList, UINT32 adwStrengthen10CoinInfo[MAX_BAG_OPEN_NUM]);//获取所有装备

    void GetAllEquip_v410(DT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI&);

	CBag&	GetBag() { return m_oBag; }
	CItemKind2IdxMap&		GetEquipKind2IdxMap() { return m_mapEquipKind2Idx; }
	CPlayerEquipShmemMap&	GetAllEquip() { return m_mapAllEquip; }
	CEquip*	GetEquip(UINT16 wEquipKindID, UINT16 wEquipIdx);
	UINT16	GetNextEquipIdx(UINT16 wItemKindID);//获取某个装备道具的idx,会 改变当前的idx
	UINT16  CkNextEquipIdx(UINT16 wItemKindID);//获取某个装备道具的idx,不会 改变当前的idx
	UINT16  GetNextJeweryIdx(UINT16 wItemKindID);//获取某个装备道具的饰品,会 改变当前的idx
	VOID	ReturnNextEquipIdx(UINT16 wItemKindID, UINT16 wIdx);//创建失败，归还idx
	//VOID	SetStrengthenPromptEquip(CEquip* poEquip);
	UINT32  GetStrenthenCountDown();//获取强化CD倒计时

	VOID OnDeleteEquip(CEquip* poEquip);//清除装备回调,内部回调OnDeleteItem
	VOID OnAddEquip(CEquip* poEquip, BOOL bHaveDress = FALSE); //新增装备回调
	VOID OnDeleteItem(CItem* poItem); //清除道具回调,内部回调有背包回调
	VOID ReduceItem( CGoods *poItem, CRecordMgr::EDecItemType emOpType ); //减少物品叠加数，如果为0，则清空, nParam1表示类型
	
	// ADD by WYX 2012-11-22 增加获取丹药个数
	VOID GetDT_DRUG_NUM(UINT16 awDrugNum[MAX_DRUG_LEVEL]);//获取丹药个数
	VOID GetDT_DRUG_NUM(UINT16 &awDrugNum, const UINT16 wDrugLeavel);//获取丹药个数
	VOID GetDT_MIX_DRUG_NUM(UINT8 byDrugNum[MAX_DRUG_LEVEL]);		//获取每品合成的丹药所需要的个数
	
	UINT16 GetDT_MIX_DRUG(UINT8 &byDrugLevel, UINT16 &wUseDrugNum, UINT16 &wProduceDrugNum);	//合成丹药

	UINT16 ClrUseDrugCD(UINT16 wKindID);

	UINT8  GetBagFullFlag();

	UINT16 SmithyBuyEquip(UINT16 wHeroKindID, UINT8 byEquipPos, DT_EQUIP_DATA_STRENGTHEN_CLI& stEquipStrengthenInfo, DT_EQUIP_DATA_CLI& stEquipInfo, UINT32& dwStrengthen10Coin);
	UINT16 ClrStrengthenCD();

	//获取装备的最大镶嵌宝石个数。
	UINT8 GetMaxMosaicGemNum();
	//获取物品占用的格子数
	UINT16 GetItemUseBagNum(UINT16 wItemID, UINT8 byNum = 1);
	//VOID	ResetStrenthenUpgradeEquipPrompt(CEquip* poEquip = NULL);//重置强化/升阶装备提示信息
public:
	////////////////////////////////////////////////饰品//////////////////////////////////////////////
	CPlayerJewelryShmemMap& GetAllJewelry() { return m_mapAllJewelry; }
	CJewelry* GetJewelry(UINT16 wJewelryID, UINT16 wIdx);
	UINT8 OnJewelryGenerateCheck(vector<UINT32>& vecKey, UINT16& wDressLevel, UINT8& byColorType, DT_RES_DATA& stRes);
	UINT16 OnJewelryGenerateInfo(PKT_CLIGS_JEWELRY_GENERATE_INFO_REQ& stReq, PKT_CLIGS_JEWELRY_GENERATE_INFO_ACK& stAck);
	UINT16 OnJewelryGenerate(PKT_CLIGS_JEWELRY_GENERATE_REQ& stReq, PKT_CLIGS_JEWELRY_GENERATE_ACK& stAck);
	UINT16 OnGetPlayerInfoJewelry(PKT_CLIGS_PLAYER_INFO_JEWELRY_ACK& stAck);
	UINT16 OnJewelryResolve(PKT_CLIGS_JEWELRY_RESOLVE_REQ& stReq, PKT_CLIGS_JEWELRY_RESOLVE_ACK& stAck);
	UINT16 OnJewelryResolve(PKT_CLIGS_JEWELRY_RESOLVE_REQ& stReq, PKT_CLIGS_JEWELRY_RESOLVE_ACK2& stAck);
	UINT16 OpenJewelryStrength(PKT_CLIGS_OPEN_JEWELRY_STRENGTH_ACK& stAck);
	VOID OnDeleteJewelry(CJewelry* poJewelry);
protected:
	BOOL	InitBag(UINT8 byBagOpenNum);//初始化背包
	BOOL	InitBagEquipItem(DT_BAG_EQUIP_DATA_LST& stBagEquipData);//初始化背包装备道具
	BOOL	InitBagGoodsItem(DT_BAG_GOODS_DATA_LST& stBagGoodsData);//初始化背包物品道具
	BOOL	InitBagJewelryItem(DT_BAG_JEWELRY_DATA_LST& stBagJewelryData);//初始化背包饰品道具
	BOOL	RecordClrUseDrugCD( INT32 nCostGold ); //记录	
	CEquip*	DressSmithyBuyEquip(CHero* poHero, UINT16 wEquipID);//穿上铁匠铺购买装备
private:
	CBag				m_oBag;					//拥有的背包	
	CItemKind2IdxMap	m_mapEquipKind2Idx;		//用于生成道具序号(大小难以控制，则使用原始map,装备Attach时重新赋值)
	CPlayerEquipShmemMap m_mapAllEquip;			//拥有保存相同类型的所有装备(穿戴的+装备背包的)
	CPlayerJewelryShmemMap m_mapAllJewelry;		//背包+穿戴
	//CEquip*				m_poMinStrengthenLevelEquip;//最小强化等级装备，用于铁匠铺是否可强化提示.(只算携带武将穿戴的装备)
	//升阶功能已废除
	//CEquip*				m_poMaxUpgradeLevelEquip;	//最大穿戴等级装备，用于铁匠铺是否可升阶提示