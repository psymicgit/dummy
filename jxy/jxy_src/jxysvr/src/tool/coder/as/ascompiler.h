#pragma once 

#include "asprotocolsource.h"
#include "asloader.h"
#include "aschecker.h"
#include "asmaker.h"

class CASCompiler
{
public:
	CASCompiler();
	~CASCompiler();

public:
	void SetOutPutPath(CString strOutPath);
	bool Compile(CString strFilePath);
	void SetErrString(CString strError);
	CString GetErrString();

protected:
	bool m_bSetOutPutPath;
	CString m_strOutPath;
	CString m_strFilePath;
	CString m_strError;

private:
	CASProtocolSource m_oProtocol;
	CASLoader m_oLoader;
	CASChecker m_oChecker;
	CASMaker m_oMaker;
};