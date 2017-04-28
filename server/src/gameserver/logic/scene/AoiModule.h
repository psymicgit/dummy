//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年4月28日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

typedef int64 ObjectId;

struct AoiObject
{
	ObjectId objId;
	float x;
	float y;
	float height;

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

class AoiModule
{
public:
	AoiModule();

public:
	bool Add(ObjectId, AoiObject*);

	bool Leave(AoiObject*);

	void Move(AoiObject*, float x, float y);

	bool PickNear(AoiObject*, float radius, std::vector<AoiObject*>&);

	bool Pick(float x, float y, float radius, std::vector<AoiObject*>&);

	void test();

private:
	std::map<ObjectId, AoiObject*> m_objs;

	std::vector<AoiObject*> m_vecX;
	std::vector<AoiObject*> m_vecY;
};