#include "sddbconfig.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

UINT32	g_dwDBLogLevel = 0;
SGDP::CSDLogger	g_oDBLogger;
bool SDAPI SGDP::SDDBSetLogger(ISDLogger* poLogger, UINT32 dwLevel)
{
	if (NULL == poLogger)
	{
		return false;
	}

	g_dwDBLogLevel = dwLevel;
	g_oDBLogger.SetLogger(poLogger);
	return true;
}

