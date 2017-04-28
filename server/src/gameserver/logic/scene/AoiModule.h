//------------------------------------------------------------------------------
// ����: ������
// ����: 2017��4��28��
// ˵��: 
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

	// Ĭ�����򣺰�x��С��������
	bool operator<(const AoiObject& other)
	{
		return x < other.x;
	}

	// ����������x��С��������
	static bool CmpX(const AoiObject* a, const AoiObject* b)
	{
		return a->x < b->x;
	}

	// ����������y��С��������
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