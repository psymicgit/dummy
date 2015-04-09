#pragma once

class CUser : public CObject
{
	DECLARE_SERIAL(CUser)

	CUser();
	virtual ~CUser();

public:
	virtual void Serialize(CArchive&);
	CUser (const CUser &user);				// copy-constructor
	CUser &operator=(const CUser &user);	// =-operator

	UINT			m_nPort;
	UINT			m_nAreaID;
	UINT			m_nPlayerID;
	CString		m_strIP;
	CString		m_strType;
	UINT			m_uParm1;
	UINT			m_uParm2;
	UINT			m_uParm3;
	UINT			m_nAreaID3;
	int			m_nUseFlag;
	CString		m_strUserName;
	CString		m_strUserPwd;
	CString		m_strServerName;
};

class CUserManager
{
public:
	CUserManager(void);
	~CUserManager(void);

	BOOL Serialize(BOOL bStoring);
	BOOL GetUser(LPCTSTR lpszConnName, CUser &user);
	void GetUserList(CArray<CUser, CUser&>&array);
	void UpdateUserList(CArray<CUser, CUser&>&array);

	BOOL AddUser(CUser &user);
	BOOL EditUser(LPCTSTR lpszIP, CUser &user);
	BOOL DelUser(LPCTSTR lpszIP);

private:
	CString				m_strFilename;
	CCriticalSection		m_CriticalSection;
	CArray<CUser, CUser&>	m_UserArray;


};

