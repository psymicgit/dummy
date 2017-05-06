#pragma once

class IModule;

class ComponentModule : public Singleton<ComponentModule>
{
public:
	bool Init();

	bool Shut();

	bool Reload();

	bool RegisterModule(const char* moduleName, IModule*);

private:
	std::vector<IModule*> m_vecModule;
	std::map<std::string, IModule*> m_mapModule;
};

#define DeclareModule(Module)											\
	Module::CreateInstance();											\
	if (!ComponentModule::instance->RegisterModule(#Module, Module::instance))		\
	{																	\
	}