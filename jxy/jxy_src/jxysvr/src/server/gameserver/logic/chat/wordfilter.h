
#ifndef WORDFILTER_H
#define WORDFILTER_H

#include <sdtype.h>
#include <map>
#include <string>
#include <string.h>
#include <db/getchatcmd.h>
#include <sdsingleton.h>

using namespace std;



/////////////////////////////////////////////////////////////////////////////
//
// WordFilter.h : interface of the CWordFilter, used to filter limited words
//                Added by Soar Chin
//
/////////////////////////////////////////////////////////////////////////////
// KMP Next values generation function
__inline void	KMP_GetNext(TCHAR * sString, int * iNext);

// KMP Match function
__inline bool	KMP_Match(const TCHAR * sString, TCHAR * sSubString, int * iNext);




class CChatMgr;
class CWordFilter
{
public:
	DECLARE_SINGLETON(CWordFilter);
public:
	BOOL Init();
	VOID UnInit();

private:
	CWordFilterDataMap	m_mapWordFilterData;
	CWordFilterDataMap	m_mapDspNameWordFilterData;
	void Free();
public:
	TCHAR	*VerifyString(const TCHAR * sString);	// Verify if the string has limited words
	bool	VerifyDspNameString(const TCHAR * sString);	// Verify if the string has limited words
	VOID	SetWordFilterData(CWordFilterDataMap &mapWordFilterData, CWordFilterDataMap &mapDspNameWordFilterData);

	CWordFilterDataMap &GetWordFilterData(){return m_mapWordFilterData;}
	CWordFilterDataMap &GetDspNameWordFilterData(){return m_mapDspNameWordFilterData;}
protected:
	BOOL	InitWordFliter();				// Init class and read from data file
	BOOL	InitDspNameWordFliter();				// Init class and read from data file
};


#endif // WORDFILTER_H
