#include "stdafx.h"
#include "ascompiler.h"

CASCompiler::CASCompiler()
{
	m_bSetOutPutPath = false;
}

CASCompiler::~CASCompiler()
{

}

void CASCompiler::SetOutPutPath( CString strOutPath )
{
	m_strOutPath = strOutPath;
	if (!m_strOutPath.IsEmpty())
	{
		m_bSetOutPutPath = true;
	}	
}

bool CASCompiler::Compile( CString strFilePath )
{
	if (!m_bSetOutPutPath)
	{
		m_strError = "Compiler Error : OutPut Path not set!";
		return false;
	}
	if(false == m_oLoader.Load(strFilePath, &m_oProtocol))
	{
		m_strError = m_oLoader.GetErrString();
		return false;
	}
	if (false == m_oChecker.Check(&m_oProtocol))
	{
		m_strError = m_oChecker.GetErrString();
		return false;
	}
	m_oMaker.SetOutPutFile(m_strOutPath);
	if (false == m_oMaker.Make(&m_oProtocol))
	{
		m_strError = m_oMaker.GetErrString();
		return false;
	}
	return true;
}

void CASCompiler::SetErrString( CString strError )
{
	m_strError = strError;
}

CString CASCompiler::GetErrString()
{
	return m_strError;
}