//------------------------------------------------------------------------------
// ����: ������
// ����: 2017��4��28��
// ˵��: 
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

	// �Ƿ��ڿ��ӷ�Χ��
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