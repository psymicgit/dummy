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

	if (m_vecX.empty())
	{
		m_vecX.push_back(obj);
		return true;
	}

	int n = m_vecX.size();
	int low = 0;
	int high = n - 1;
	int mid = 0;

	while (low <= high)
	{
		mid = (low + high) / 2;

		AoiObject* midObj = m_vecX[mid];

		if (midObj->x <= obj->x)
		{
			low = mid + 1;
		}
		else
		{
			high = mid - 1;
		}
	}

	m_vecX.insert(m_vecX.begin() + low, obj);

	/*
	m_vecX.push_back(obj);
	m_vecY.push_back(obj);

	std::sort(m_vecX.begin(), m_vecX.end(), AoiObject::CmpX);
	std::sort(m_vecY.begin(), m_vecY.end(), AoiObject::CmpY);
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

	utiltool::VectorErase(m_vecX, obj);
	utiltool::VectorErase(m_vecY, obj);
}

void AoiModule::Move(AoiObject* obj, float x, float y)
{
	obj->x = x;
	obj->y = x;

	std::sort(m_vecX.begin(), m_vecX.end(), AoiObject::CmpX);
	std::sort(m_vecY.begin(), m_vecY.end(), AoiObject::CmpY);
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

	for (int i = 1; i <= 100000; ++i)
	{
		AoiObject* obj = new AoiObject;
		obj->objId = i;
		obj->x = randtool::rand_float(range);
		obj->y = obj->x / 2;

		Add(i, obj);
	}

	for (int i = 0, n = (int)m_vecX.size(); i < n; ++i)
	{
		AoiObject* obj = m_vecX[i];
		Move(obj, randtool::rand_float(range), randtool::rand_float(range));
	}
}