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


        ar <<		m_nPort;
        ar <<		m_nAreaID;
        ar <<		m_nPlayerID;
        ar <<		m_strIP;
        ar <<		m_strType;
        ar <<		m_uParm1;
        ar <<		m_uParm2;
        ar <<		m_uParm3;
        ar <<		m_nAreaID3;

        ar <<		m_nUseFlag;
        ar <<		m_strUserName;
        ar <<		m_strUserPwd;
        ar <<		m_strServerName;
    }
    else
    {
        // 'load' data
        ar >>		m_nPort;
        ar >>		m_nAreaID;
        ar >>		m_nPlayerID;
        ar >>		m_strIP;
        ar >>		m_strType;
        ar >>		m_uParm1;
        ar >>		m_uParm2;
        ar >>		m_uParm3;
        ar >>		m_nAreaID3;

        ar >>		m_nUseFlag;
        ar >>		m_strUserName;
        ar >>		m_strUserPwd;
        ar >>		m_strServerName;
    }
}


/* Copy-constructor */
CUser::CUser(const CUser &user)
{
    *this = user;
}

/* = operator definition */
CUser& CUser::operator=(const CUser &user)
{
    if (&user != this)
    {
        m_nPort = user.m_nPort;
        m_nAreaID = user.m_nAreaID;
        m_nPlayerID = user.m_nPlayerID;
        m_strIP = user.m_strIP;
        m_strType = user.m_strType;
        m_uParm1 = user.m_uParm1;
        m_uParm2 = user.m_uParm2;
        m_uParm3 = user.m_uParm3;
        m_nAreaID3 = user.m_nAreaID3;
        m_nUseFlag = user.m_nUseFlag;
        m_strUserName = user.m_strUserName;
        m_strUserPwd = user.m_strUserPwd;
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
BOOL CUserManager::GetUser(LPCTSTR lpszConn, CUser &user)
{
    m_CriticalSection.Lock();
    for (int i = 0; i < m_UserArray.GetSize(); i++)
    {
        if (!m_UserArray[i].m_strIP.CompareNoCase(lpszConn))
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
        if (0 == m_UserArray[i].m_strIP.CompareNoCase(user.m_strIP))
        {
            m_CriticalSection.Unlock();

			EditUser(m_UserArray[i].m_strIP, user);
            return FALSE;
        }
    }
    m_UserArray.Add(user);
    m_CriticalSection.Unlock();
    Serialize(TRUE);
    return TRUE;
}

BOOL CUserManager::EditUser(LPCTSTR lpszIP, CUser &user)
{
    m_CriticalSection.Lock();
    for (int i = 0; i < m_UserArray.GetSize(); i++)
    {
        if (0 == m_UserArray[i].m_strIP.CompareNoCase(lpszIP))
        {
            m_UserArray[i] = user;
            m_CriticalSection.Unlock();
            Serialize(TRUE);
            return TRUE;
        }
    }
    m_CriticalSection.Unlock();
    return TRUE;
}


BOOL CUserManager::DelUser(LPCTSTR lpszIP)
{
    m_CriticalSection.Lock();
    for (int i = 0; i < m_UserArray.GetSize(); i++)
    {
        if (0 == m_UserArray[i].m_strIP.CompareNoCase(lpszIP))
        {
            m_UserArray.RemoveAt(i);
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

