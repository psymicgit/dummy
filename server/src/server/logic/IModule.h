#pragma once

class IModule
{
public:
	virtual bool Init() = 0;

	virtual bool Shut() { return true; }

	virtual bool Reload() { return true; }
};