//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年4月28日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#include "AoiModule.h"
#include <tool/utiltool.h>
#include <tool/randtool.h>

AoiModule::AoiModule()
{

}

bool AoiModule::Add(ObjectId objId, AoiObject *obj)
{
	if (utiltool::Has(m_objs, objId))
	{
		return false;
	}

	m_objs[objId] = obj;
	int at = GetInsertPos(obj->x, obj->y);
	m_vecX.insert(m_vecX.begin() + at, obj);

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
}

void AoiModule::Move(AoiObject* obj, float x, float y)
{
	int old_pos = GetObjPos(obj);
	int new_pos = GetInsertPos(x, y);

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

int AoiModule::GetInsertPos(float x, float y)
{
	int n = m_vecX.size();
	if (n <= 0)
	{
		return 0;
	}

	int low = 0;
	int high = n - 1;
	int mid = 0;

	while (low <= high)
	{
		mid = (low + high) / 2;

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

void AoiModule::SetPos(int from, int to)
{
	int n = (int)m_vecX.size();
	if (from <= 0 || from > n || to <= 0 || to > n)
	{
		return;
	}

	for (int i = from; i <= to; ++i)
	{
		//m_vecX[i - 1]->pos = i;
	}
}

bool AoiModule::PickNear(AoiObject* obj, float radius, std::vector<AoiObject*>& outs)
{
	return Pick(obj->x, obj->y, radius, outs);
}

bool AoiModule::Pick(float x, float y, float radius, std::vector<AoiObject*>& outs)
{
	int n = m_vecX.size();
	int low = 1;
	int high = n;

	float bound_left = x - radius;
	float bound_right = x + radius;

	while (low <= high)
	{
		int mid = (low + high) / 2;
		AoiObject* midObj = m_vecX[mid];

		if (midObj->x < bound_left)
		{
			low = mid + 1;
		}
		else if (midObj->x > bound_left)
		{
			high = mid - 1;
		}
		else
		{
			break;
		}
	}

	for (int i = low; i <= high; ++i)
	{
		AoiObject* obj = m_vecX[i];

		if (utiltool::Between(obj->x, bound_left, bound_right))
		{
			outs.push_back(obj);
		}
	}
}

void AoiModule::test()
{
	const float range = 1000.0f;

	float init[] =
	{
		1, 5, 8, 19, 19,
		19, 40, 50, 61, 62
	};

	for (int i = 1; i <= 100000; ++i)
	{
		AoiObject* obj = new AoiObject;
		obj->objId = i;
		obj->x = randtool::rand_float(range);
		obj->y = obj->x / 2;
		//obj->x = init[i - 1];

		Add(i, obj);
	}

	Move(m_vecX[2], 19.0f, 0);
	Move(m_vecX[4], 20, 0);

	for (int i = 0, n = (int)m_vecX.size(); i < n; ++i)
	{
		AoiObject* obj = m_vecX[i];
		Move(obj, randtool::rand_float(range), randtool::rand_float(range));
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
