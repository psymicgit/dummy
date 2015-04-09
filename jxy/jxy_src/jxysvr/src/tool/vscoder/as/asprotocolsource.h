#pragma once 
#include <map>
#include <vector>
#include <string>
using namespace std;

class CASDefine
{
public:
	CASDefine(){}
	~CASDefine(){}
public:
	inline void SetName(CString& strName) { m_strName = strName; }
	inline void SetValue(CString& strValue) { m_strValue = strValue; }
	inline CString& GetName() { return m_strName; }
	inline CString& GetValue() { return m_strValue; }
protected:
	CString m_strName;
	CString m_strValue;
};

enum EItemType
{
	EI_None,
	EI_SimpleType,	// simple -> (byte, word, dword, string, uint64)
	EI_StructType,
	EI_StructListType,
};
class CASTypeItem
{
public:
	CASTypeItem():m_eItemType(EI_None), m_bIsArray(false){}
	~CASTypeItem(){}

public:
	inline void SetName(CString& strName) { m_strName = strName; }
	inline void SetType(CString& strType) { m_strType = strType; }
	inline void SetRefer(CString& strRefer) { m_strRefer = strRefer; }
	inline void SetCount(CString& strCount) { m_strCount = strCount ;}
	inline CString& GetName() { return m_strName; }
	inline CString& GetType() { return m_strType; }
	inline CString& GetRefer() { return m_strRefer; }
	inline CString& GetCount() { return m_strCount; }
	// for write to files
	inline void SetVarType(CString strVarType){ m_strVarType = strVarType;}
	inline void SetConvType(CString strConvType){ m_strConvType = strConvType;}
	inline CString& GetVarType(){ return m_strVarType;}
	inline CString& GetConvType(){ return m_strConvType;}
	inline void SetItemType(EItemType eIT){ m_eItemType = eIT;}
	inline EItemType GetItemType() { return m_eItemType;}
	inline void SetArray(bool bIsArray) {m_bIsArray = bIsArray;}
	inline bool IsArray() { return m_bIsArray;}
	inline void SetLength(CString strLength) {m_strLength = strLength;}
	inline CString& GetLength() {return m_strLength;}
protected:
	CString m_strName;
	CString m_strType;
	CString m_strRefer;
	CString m_strCount;
	CString m_strVarType;
	CString m_strConvType;
	EItemType m_eItemType;
	bool m_bIsArray;	// if item is a simple type and not a string, and count is a valid value, means its a array

	// if count or refer is valid, m_strLength record the real one need be write, 
	//eg. count = LG_PTID_LEN, refer = null, m_strLength = classname.LG_PTID_LEN
	//eg. count = some_value, refer = a_valid_item_name, m_strLength = a_valid_item_name
	CString m_strLength;	
};

class CASType
{
	typedef std::map<CString, CASTypeItem*> CItemMap;
	typedef std::vector<CASTypeItem*> CItemVec;
public:
	CASType():m_bIsStructType(false),m_bIsStructArrayType(false), m_bGenerateClass(false){}
	~CASType(){}

	bool InsertItem(CASTypeItem* rTypeItem);
	CASTypeItem* GetItem(CString& strName);
	inline void SetName(CString& strName) {m_strName = strName;}
	CString& GetName() {return m_strName;}
	CASTypeItem* GetItemByPos(int nPos);
	int GetItemCount() { return (int)m_vecTypeItems.size();}
	inline void SetStructType(bool bIsStruct){ m_bIsStructType = bIsStruct;}
	inline void SetStructArrayType(bool bIsStructArray){ m_bIsStructArrayType = bIsStructArray;}
	inline bool IsStructType(){ return m_bIsStructType;}
	inline bool IsStructArrayType(){ return m_bIsStructArrayType;}
	inline void SetGenerateClass(bool bGenerate) { m_bGenerateClass = bGenerate;}
	inline bool IsGenerateClass(){ return m_bGenerateClass;}
protected:
	CString m_strName;
	CItemMap m_mapTypeItems;
	CItemVec m_vecTypeItems;
	bool m_bIsStructType;
	bool m_bIsStructArrayType;
	bool m_bGenerateClass;
};

class CASMessage
{
public:
	CASMessage():m_bBoth(false){}
	~CASMessage(){}

public:
	inline void SetName(CString& strName) { m_strName = strName; }
	inline void SetID(CString& strID) { m_strID = strID; }
	inline void SetType(CString& strType) { m_strType = strType; }
	inline void SetDir(CString& strDir) { m_strDir = strDir; }
	inline void SetInfo(CString& strInfo) { m_strInfo = strInfo; }
	inline CString& GetName() { return m_strName;}
	inline CString& GetID() { return m_strID; }
	inline CString& GetType() { return m_strType; }
	inline CString& GetDir() { return m_strDir;}
	inline CString& GetInfo() { return m_strInfo; }
	inline void SetBoth(bool bValue) {m_bBoth = bValue;}
	inline bool IsBoth(){return m_bBoth;}
protected:
	CString m_strName;
	CString m_strID;
	CString m_strType;
	CString m_strDir;
	CString m_strInfo;
	bool m_bBoth;
};

typedef std::map<string, CASDefine*> CDefineMap;
typedef std::map<string, CASMessage*> CMessageMap;
typedef std::map<string, CASType*> CTypeMap;

typedef std::vector<CASDefine*> CDefineVec;
typedef std::vector<CASType*> CTypeVec;
typedef std::vector<CASMessage*>CMessageVec;

typedef std::vector<CString> CIncludeVec;

class CASProtocolSource
{
public:
	CASProtocolSource():m_strPackageName(""),m_strClassName(""),m_strVersion(""){}
	~CASProtocolSource(){}

public:
	inline void SetPackageName(CString& strName){ m_strPackageName = strName; }
	inline void SetClassName(CString& strName){m_strClassName = strName;}
	inline void SetVersion(CString&strVersion){m_strVersion = strVersion;}
	inline CString& GetPackageName() {return m_strPackageName;}
	inline CString& GetClassName(){return m_strClassName;}
	inline CString& GetVersion(){return m_strVersion;}

	bool InsertDefine(CASDefine* pDefine);
	bool InsertType(CASType* pType);
	bool InsertCSMessage(CASMessage* pMessage);
	bool InsertSCMessage(CASMessage* pMessage);	

	bool InsertImport(CString strImport);
	
	int GetDefineCount();
	int GetTypeCount();
	int GetCSMessageCount();
	int GetSCMessageCount();

	int GetImportCount();

	CASDefine* GetDefineByPos(int nPos);
	CASType* GetTypeByPos(int nPos);
	CASMessage* GetCSMessageByPos(int nPos);
	CASMessage* GetSCMessageByPos(int nPos);
	CASType* GetStructTypeByPos(int nPos);
	CASType* GetStructArrayTypeByPos(int nPos);

	CString GetImportByPos(int nPos);

	CASDefine* FindDefineByName(CString& strName);
	CASType* FindTypeByName(CString& strName);
	CASMessage* FindCSMessageByName(CString& strName);
	CASMessage* FindSCMessageByName(CString& strName);

	bool IsDefineExist(CString& strName);
	bool IsTypeExist(CString& strName);
	bool IsSCMessageExist(CString& strName);
	bool IsCSMessageExist(CString& strName);

	CString& GetErrString(){ return m_strError;}
protected:
	CString m_strPackageName;
	CString m_strClassName;
	CString m_strVersion;
	CIncludeVec m_vecInclude;

	CDefineMap m_mapDefines;
	CMessageMap m_mapCSMessages;
	CMessageMap m_mapSCMessages;
	CTypeMap m_mapTypes;

	CDefineVec m_vecDefines;
	CTypeVec m_vecTypes;
	CMessageVec m_vecCSMessages;
	CMessageVec m_vecSCMessages;
	
	CString m_strError;
};