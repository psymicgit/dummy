//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年4月28日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#include "AoiModule.h"
#include <tool/utiltool.h>
#include <tool/randtool.h>
#include <tool/timetool.h>

AoiModule::AoiModule()
{

}

bool AoiModule::Init()
{
	m_allocObjId = 0;
	return true;
}

void AoiModule::Update()
{

}

bool AoiModule::Add(AoiObject *obj)
{
	if (utiltool::Has(m_objs, obj->objId))
	{
		return false;
	}

	m_objs[obj->objId] = obj;
	int at = GetInsertPos(obj->x, obj->y);
	m_vecX.insert(m_vecX.begin() + at, obj);

	/*
	for (int i = 0, n = m_vecX.size(); i < n; ++i)
	{
		AoiObject* other = m_vecX[i];
		if (IsInSeeRange(other, obj->x))
		{
			
		}
	}
	*/

	return true;
}

bool AoiModule::Leave(AoiObject* obj)
{
	auto itr = m_objs.find(obj->objId);
	if (itr == m_objs.end())
	{
		return false;
	}

	m_objs.erase(itr);

	int pos = GetObjPos(obj);
	if (pos >= 0)
	{
		m_vecX.erase(m_vecX.begin() + pos);
	}

	return true;
}

void AoiModule::Move(AoiObject* obj, float x, float y)
{
	if (0)
	{
		Leave(obj);
		Add(obj);
	}
	else
	{
		int old_pos = GetObjPos(obj);
		int new_pos = GetInsertPos(x, 0);

		obj->x = x;
		obj->y = y;

		if (old_pos == new_pos || old_pos < 0)
		{
			return;
		}

		if (new_pos < old_pos)
		{
			memmove(&m_vecX[new_pos + 1], &m_vecX[new_pos], (old_pos - new_pos) * sizeof(AoiObject*));
			m_vecX[new_pos] = obj;
		}
		else
		{
			memmove(&m_vecX[old_pos], &m_vecX[old_pos + 1], (new_pos - old_pos) * sizeof(AoiObject*));
			m_vecX[new_pos - 1] = obj;
		}
	}
}

int AoiModule::GetInsertPos(float x, float y)
{
	int n = (int)m_vecX.size();
	return GetInsertPosBetween(x, 0, n);
}

int AoiModule::GetInsertPosBetween(float x, int from, int to)
{
	if (to <= 0)
	{
		return 0;
	}

	int low = from;
	int high = to - 1;

	while (low <= high)
	{
		int mid = (low + high) / 2;

		AoiObject* midObj = m_vecX[mid];

		if (midObj->x <= x)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}

	return low;
}

int AoiModule::GetObjPos(AoiObject* obj)
{
	int n = (int)m_vecX.size();

	int at = GetInsertPos(obj->x, obj->y);
	at = __min(at, n - 1);

	// 向前依次搜索
	for (int i = at; i >= 0; --i)
	{
		AoiObject* other = m_vecX[i];
		if (other == obj)
		{
			return i;
		}

		if (other->x < obj->x)
		{
			break;
		}
	}

	// 向右依次搜索
	for (int i = at + 1; i < n; ++i)
	{
		AoiObject* other = m_vecX[i];
		if (other == obj)
		{
			return i;
		}

		if (other->x > obj->x)
		{
			break;
		}
	}

	return -1;
}

bool AoiModule::PickNear(AoiObject* obj, float radius, std::vector<AoiObject*>& outs)
{
	return Pick(obj->x, obj->y, radius, outs);
}

bool AoiModule::PickKen(ObjectId aoiObjId, std::vector<AoiObject*>& outs)
{
	AoiObject* obj = FindObject(aoiObjId);
	if (nullptr == obj)
	{
		return false;
	}

	return Pick(obj->x, obj->y, 1000, outs);
}

bool AoiModule::PickKenByPos(float x, float y, std::vector<AoiObject*>& outs)
{
	return Pick(x, y, 1000, outs);
}

bool AoiModule::Pick(float x, float y, float radius, std::vector<AoiObject*>& outs)
{
	float bound_left = x - radius;
	float bound_right = x + radius;

	int leftPos = GetInsertPos(x, y);

	int n = m_vecX.size();

	for (int i = leftPos; i < n; ++i)
	{
		AoiObject* obj = m_vecX[i];

		if (utiltool::Between(obj->x, bound_left, bound_right))
		{
			outs.push_back(obj);
		}
		else
		{
			break;
		}
	}

	for (int i = leftPos - 1; i >= 0; --i)
	{
		AoiObject* obj = m_vecX[i];

		if (utiltool::Between(obj->x, bound_left, bound_right))
		{
			outs.push_back(obj);
		}
		else
		{
			break;
		}
	}

	return !outs.empty();
}

// 是否在可视范围内
bool AoiModule::IsInSeeRange(AoiObject* obj, float x)
{
	return utiltool::Between(x, obj->x - obj->seeRadius, obj->x + obj->seeRadius);
}

void AoiModule::test()
{
	const float range = 1000.0f;
	int n = 10000;
	int pickNum = 1000;

	float init[] =
	{
		1, 5, 8, 19, 19,
		19, 40, 50, 61, 62
	};

	//*********** 添加 ***********//

	Timestamp time1 = timetool::now();

	for (int i = 1; i <= n; ++i)
	{
		AoiObject* obj = new AoiObject;
		obj->objId = i;
		obj->x = randtool::rand_float(range);
		obj->y = obj->x / 2;
		//obj->x = init[i - 1];

		Add(obj);
	}

	//*********** 移动 ***********//

	Timestamp time2 = timetool::now();

	int sum_add = (int)(time2 - time1);
	float avg_add = (float)sum_add / n;
	LOG_INFO << "sum_add = " << sum_add << ", avg_add = " << avg_add;

	for (int i = 0, n = (int)m_vecX.size(); i < n; ++i)
	{
		AoiObject* obj = m_vecX[i];
		Move(obj, randtool::rand_float(range), randtool::rand_float(range));
	}

	Timestamp end = timetool::now();

	int sum_move = (int)(end - time2);
	float avg_move = (float)sum_move / n;
	LOG_INFO << "sum_move = " << sum_move << ", avg_move = " << avg_move;

	//*********** 搜索附近对象 ***********//

	std::vector<AoiObject*> picks;

	for (int i = 0; i < pickNum; ++i)
	{
		Pick(randtool::rand_float(range), 0, 100, picks);
		picks.clear();
	}

	if (!IsOk())
	{
		return;
	}
}

bool AoiModule::IsOk()
{
	int n = (int)m_vecX.size();
	if (n <= 1)
	{
		return true;
	}

	for (int i = 0; i < n - 1; ++i)
	{
		if (m_vecX[i]->x > m_vecX[i + 1]->x)
		{
			return false;
		}
	}

	return true;
}

int AoiModule::AllocObjId()
{
	++m_allocObjId;
	m_allocObjId = __max(m_allocObjId, 1);

	return m_allocObjId;
}

AoiObject* AoiModule::FindObject(ObjectId aoiObjId)
{
	auto itr = m_objs.find(aoiObjId);
	if (itr == m_objs.end())
	{
		return nullptr;
	}

	return itr->second;
}

const std::vector<AoiObject*>& AoiModule::GetObjs()
{
	return m_vecX;
}
