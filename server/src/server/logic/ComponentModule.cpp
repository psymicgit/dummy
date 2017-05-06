//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年5月6日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#include "ComponentModule.h"
#include "IModule.h"
#include "tool/utiltool.h"

bool ComponentModule::Init()
{
	bool ok = true;

	for (int i = 0, n = m_vecModule.size(); i < n; ++i)
	{
		IModule* module = m_vecModule[i];
		if (!module->Init())
		{
			ok = false;
		}
	}

	return ok;
}

bool ComponentModule::Shut()
{
	bool ok = true;

	for (int i = 0, n = m_vecModule.size(); i < n; ++i)
	{
		IModule* module = m_vecModule[i];
		if (!module->Shut())
		{
			ok = false;
		}
	}

	return ok;
}

bool ComponentModule::Reload()
{
	bool ok = true;

	for (int i = 0, n = m_vecModule.size(); i < n; ++i)
	{
		IModule* module = m_vecModule[i];
		if (!module->Reload())
		{
			ok = false;
		}
	}

	return ok;
}

bool ComponentModule::RegisterModule(const char* moduleName, IModule* module)
{
	if (utiltool::Has(m_mapModule, moduleName))
	{
		return false;
	}

	m_mapModule[moduleName] = module;
	m_vecModule.push_back(module);

	return true;
}
