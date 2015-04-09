#pragma once


#include <sddb.h>


class CUser : public CObject
{
	DECLARE_SERIAL(CUser)

	CUser();
	virtual ~CUser();

public:
	virtual void Serialize(CArchive&);
	CUser (const CUser &user);				// copy-constructor
	CUser &operator=(const CUser &user);	// =-operator

	CString			m_strConnName;
	SGDP::SDDBAccount	m_stAccount;
};

class CUserManager
{
public:
	CUserManager(void);
	~CUserManager(void);

	BOOL Serialize(BOOL bStoring);
	BOOL GetUser(LPCTSTR lpszUser, CUser &user);
	void GetUserList(CArray<CUser, CUser&>&array);
	void UpdateUserList(CArray<CUser, CUser&>&array);

	BOOL AddUser(CUser &user);
	BOOL EditUser(LPCTSTR lpszUser, CUser &user);

private:
	CString				m_strFilename;
	CCriticalSection		m_CriticalSection;
	CArray<CUser, CUser&>	m_UserArray;


};

