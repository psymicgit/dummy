#include "StdAfx.h"
#include "User.h"

IMPLEMENT_SERIAL(CUser, CObject, 1)

CUser::CUser()
{

}

CUser::~CUser()
{

}


void CUser::Serialize(CArchive& ar)
{
    CString strTemp = "";
    if (ar.IsStoring())
    {
        // 'store' data
        ar << m_strConnName;
        ar << CString(m_stAccount.m_szHostName);
        ar << CString(m_stAccount.m_szDBName);
        ar << CString(m_stAccount.m_szLoginName);
        ar << CString(m_stAccount.m_szLoginPwd);
        ar << CString(m_stAccount.m_szCharactSet);
        ar << CString(m_stAccount.m_szUnixSocket);
        ar << m_stAccount.m_wConnPort;
        ar << m_stAccount.m_wDBType;
    }
    else
    {
        // 'load' data
        ar >> m_strConnName;
        ar >> strTemp;
        memcpy(m_stAccount.m_szHostName, strTemp.GetBuffer(0), strTemp.GetLength());
        ar >> strTemp;
        memcpy(m_stAccount.m_szDBName, strTemp.GetBuffer(0), strTemp.GetLength());
        ar >> strTemp;
        memcpy(m_stAccount.m_szLoginName, strTemp.GetBuffer(0), strTemp.GetLength());
        ar >> strTemp;
        memcpy(m_stAccount.m_szLoginPwd, strTemp.GetBuffer(0), strTemp.GetLength());
        ar >> strTemp;
        memcpy(m_stAccount.m_szCharactSet, strTemp.GetBuffer(0), strTemp.GetLength());
        ar >> strTemp;
        memcpy(m_stAccount.m_szUnixSocket, strTemp.GetBuffer(0), strTemp.GetLength());
        ar >> m_stAccount.m_wConnPort;
        ar >> m_stAccount.m_wDBType;
    }
}


/* Copy-constructor */
CUser::CUser(const CUser &user)
{
    m_strConnName = user.m_strConnName;
}

/* = operator definition */
CUser& CUser::operator=(const CUser &user)
{
    if (&user != this)
    {
        m_strConnName = user.m_strConnName;
        m_stAccount = user.m_stAccount;
    }
    return *this;
}




CUserManager::CUserManager()
{
    GetAppDir(m_strFilename);
    m_strFilename += "users.dat";
}

CUserManager::~CUserManager()
{

}


/********************************************************************/
/*																	*/
/* Function name : Serialize										*/
/* Description   : Call this function to store/load the user data	*/
/*																	*/
/********************************************************************/
BOOL CUserManager::Serialize(BOOL bStoring)
{
    static const TCHAR* lpszSignature = _T("MaxNet Software Solutions - StoreObject");

    CFile file;

    if (file.Open(m_strFilename, bStoring ? CFile::modeWrite | CFile::modeCreate : CFile::modeRead))
    {
        TRY
        {
            CString str;
            CArchive ar(&file, bStoring ? CArchive::store : CArchive::load);

            if (bStoring)
            {
                // save signature
                ar << CString(lpszSignature);

                // Save the changed user details
                for (int i = 0; i < m_UserArray.GetSize(); i++)
                {
                    m_UserArray[i].Serialize(ar);
                }

                ar.Flush();
            }
            else
            {
                // load signature
                ar >> str;
                // if this the file we are looking for ?
                if (str.Compare(lpszSignature) == 0)
                {
                    int nCount = 0;

                    while(!ar.IsBufferEmpty())
                    {
                        CUser user;

                        // get user data
                        user.Serialize(ar);

                        // add user to array
                        m_UserArray.Add(user);
                    }
                }
            }
            ar.Close();
            file.Close();
        }
        CATCH_ALL(e)
        {
            // catch all exceptions that might happen ...
            return FALSE;
        }
        END_CATCH_ALL
    }
    return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : GetUser											*/
/* Description   : Get user object for specified username			*/
/*																	*/
/********************************************************************/
BOOL CUserManager::GetUser(LPCTSTR lpszUser, CUser &user)
{
    m_CriticalSection.Lock();
    for (int i = 0; i < m_UserArray.GetSize(); i++)
    {
        if (!m_UserArray[i].m_strConnName.CompareNoCase(lpszUser))
        {
            user = m_UserArray[i];
            m_CriticalSection.Unlock();
            return TRUE;
        }
    }
    m_CriticalSection.Unlock();
    return FALSE;
}

BOOL CUserManager::AddUser(CUser &user)
{
    m_CriticalSection.Lock();
    for (int i = 0; i < m_UserArray.GetSize(); i++)
    {
        if (0 == m_UserArray[i].m_strConnName.CompareNoCase(user.m_strConnName))
        {
            m_CriticalSection.Unlock();
            return FALSE;
        }
    }
    m_UserArray.Add(user);
    m_CriticalSection.Unlock();
    Serialize(TRUE);
    return TRUE;
}

BOOL CUserManager::EditUser(LPCTSTR lpszUser, CUser &user)
{
    m_CriticalSection.Lock();
    for (int i = 0; i < m_UserArray.GetSize(); i++)
    {
        if (0 == m_UserArray[i].m_strConnName.CompareNoCase(lpszUser))
        {
            m_UserArray[i].m_strConnName = user.m_strConnName;
            m_UserArray[i].m_stAccount.Init();
            m_UserArray[i].m_stAccount = user.m_stAccount;
            m_CriticalSection.Unlock();
            Serialize(TRUE);
            return TRUE;
        }
    }
    m_CriticalSection.Unlock();
    return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name : GetUserList										*/
/* Description   : Make copy of user array							*/
/*																	*/
/********************************************************************/
void CUserManager::GetUserList(CArray<CUser, CUser&>&array)
{
    m_CriticalSection.Lock();
    for (int i = 0; i < m_UserArray.GetSize(); i++)
    {
        array.Add(m_UserArray[i]);
    }
    m_CriticalSection.Unlock();
}


/********************************************************************/
/*																	*/
/* Function name : UpdateUserList									*/
/* Description   : Update user array								*/
/*																	*/
/********************************************************************/
void CUserManager::UpdateUserList(CArray<CUser, CUser&>&array)
{
    m_CriticalSection.Lock();
    m_UserArray.RemoveAll();
    for (int i = 0; i < array.GetSize(); i++)
    {
        m_UserArray.Add(array[i]);
    }
    m_CriticalSection.Unlock();
    Serialize(TRUE);
}

