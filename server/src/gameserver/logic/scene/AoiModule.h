//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年4月28日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

struct AoiObject
{
	ObjectId objId;
	float x;
	float y;
	float height;
	float seeRadius;
	int clientId;

	// 默认排序：按x从小到大排序
	bool operator<(const AoiObject& other)
	{
		return x < other.x;
	}

	// 排序函数：按x从小到大排序
	static bool CmpX(const AoiObject* a, const AoiObject* b)
	{
		return a->x < b->x;
	}

	// 排序函数：按y从小到大排序
	static bool CmpY(const AoiObject* a, const AoiObject* b)
	{
		return a->y < b->y;
	}
};

class AoiModule : public IModule, public Singleton<AoiModule>
{
public:
	AoiModule();

public:
	bool Init();

	void Update();

	bool Add(AoiObject*);

	bool Leave(AoiObject*);

	void Move(AoiObject*, float x, float y);

	int GetInsertPos(float x, float y);

	inline int GetInsertPosBetween(float x, int from, int to);

	int GetObjPos(AoiObject*);

	bool PickNear(AoiObject*, float radius, std::vector<AoiObject*>&);

	bool PickKen(ObjectId, std::vector<AoiObject*>&);

	bool PickKenByPos(float x, float y, std::vector<AoiObject*>&);

	bool Pick(float x, float y, float radius, std::vector<AoiObject*>&);

	// 是否在可视范围内
	bool IsInSeeRange(AoiObject*, float x);

	void test();

	bool IsOk();

	int AllocObjId();

	AoiObject* FindObject(ObjectId);

	const std::vector<AoiObject*>& GetObjs();

private:
	std::map<ObjectId, AoiObject*> m_objs;

	std::vector<AoiObject*> m_vecX;
	std::vector<AoiObject*> m_vecY;

	int m_allocObjId;
};