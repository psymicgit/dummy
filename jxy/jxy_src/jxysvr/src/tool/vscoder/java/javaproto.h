#ifndef javaproto_h
#define javaproto_h

#include <list>

/**
 * imported file management
 *
 * like: 
 *   <include>
 *       <import name="com.right.socket.Handler_Base" /> -> CJavaInclude
 *       ...
 *   </include>
 */

class CJavaInclude
{
public:
    
    /**
     * ctor
     */

    CJavaInclude() {}

    /**
     * dtor
     */

    ~CJavaInclude() {}

    /**
     * set imported file name
     *
     * @param strInclude imported file name CString
     */

    inline void SetInclude(CString& strInclude) { m_strInclude = strInclude; }

    /**
     * get imported file name CString
     */

    inline CString& GetInclude() { return m_strInclude; }

private:
    CString m_strInclude; ///< imported file name CString
};

/**
 * JAVA define
 *
 * like:
 *   <defines>
 *     <define name="ID_LEN" value="40" /> -> CJavaDefine
 *     ...
 *   </defines>
 */

class CJavaDefine
{
public:

    /**
     * ctor
     */
    
	CJavaDefine(){}

    /**
     * dtor
     */

	~CJavaDefine(){}

    /**
     * set define name
     *
     * @param strName define name CString
     */

	inline void SetName(CString& strName) { m_strName = strName; }

    /**
     * set define value
     *
     * @param strValue define value CString
     */

	inline void SetValue(CString& strValue) { m_strValue = strValue; }

    /**
     * get define name CString
     *
     * @return define name CString
     */

	inline CString& GetName() { return m_strName; }

    /**
     * get define value CString
     *
     * @return define value CString
     */

	inline CString& GetValue() { return m_strValue; }

protected:
	CString m_strName;   ///< define name CString
	CString m_strValue;  ///< define value CString
};

/**
 * JAVA message
 *
 * like:
 *   <messages>
 *     <message name="CLIGS_ENTERROOM_REQ" id="2001" type="PKG_CLIGS_ENTERROOM_REQ" /> -> CJavaMessage
 *     ...
 *   </messages>
 */

class CJavaMessage
{
public:

    /**
     * ctor
     */

	CJavaMessage() {}

    /**
     * dtor
     */

	~CJavaMessage() {}

    /**
     * set message name
     *
     * @param strName message name CString
     */

	inline void SetName(CString& strName) { m_strName = strName; }

    /**
     * set message ID
     *
     * @param strID message ID CString
     */

	inline void SetID(CString& strID) { m_strID = strID; }

    /**
     * set message type
     *
     * @param strType message type CString
     */

	inline void SetType(CString& strType) { m_strType = strType; }

    /**
     * get message name
     *
     * @return message name CString
     */

	inline CString& GetName() { return m_strName;}

    /**
     * get message ID
     *
     * @return message ID CString
     */

	inline CString& GetID() { return m_strID; }

    /**
     * get message type
     *
     * @return message type CString
     */

	inline CString& GetType() { return m_strType; }

protected:
	CString m_strName; ///< message name CString
	CString m_strID;   ///< message ID CString
	CString m_strType; ///< message type CString
};

/**
 * JAVA type item
 *
 * like:
 *   <types>
 *      <type name=""demo">
 *        <item name="Step" type="int" /> -> CJavaTypeItem
 *        ...
 *      </type>
 *      ...
 *   </types>
 */

class CJavaTypeItem
{
public:

    /**
     * ctor
     */

    CJavaTypeItem() {}

    /**
     * dtor
     */

    ~CJavaTypeItem() {}

    // MUST HAS attributes

    /**
     * set item name
     *
     * @param strName item name CString
     */

    inline void SetName(CString& strName) { m_strName = strName; }

    /**
     * set item type
     *
     * @param strType item type CString
     */

    inline void SetType(CString& strType) { m_strType = strType; }

    /**
     * get item name
     *
     * @return item name CString
     */

    inline CString& GetName() { return m_strName; }

    /**
     * get item type
     *
     * @return item type CString
     */

    inline CString& GetType() { return m_strType; }

    // optional attributes

    /**
     * set item count
     *
     * @param strCount item count CString
     */

    inline void SetCount(CString& strCount) { m_strCount  = strCount;  }

    /**
     * set item refer
     *
     * @param strRefer item refer CString
     */

    inline void SetRefer(CString& strRefer) { m_strRefer  = strRefer;  }

    /**
     * set item select, reserved
     *
     * @param strSelect item select CString
     */

    inline void SetSelect(CString& strSelect) { m_strSelect = strSelect; }

    /**
     * set item ID
     *
     * @param strId item ID CString
     */

    inline void SetId(CString& strId) { m_strId     = strId;     }

    /**
     * get item count
     *
     * @return item count CString
     */

    inline CString& GetCount() { return m_strCount;  }

    /**
     * get item refer
     *
     * @return item refer CString
     */

    inline CString& GetRefer() { return m_strRefer;  }

    /**
     * get item select, reserved
     *
     * @return item select CString
     */

    inline CString& GetSelect() { return m_strSelect; }

    /**
     * get item ID
     *
     * @return item ID CString
     */

    inline CString& GetId() { return m_strId;     }

protected:

    // MUST HAS attributes

    CString m_strName;  ///< item name CString
    CString m_strType;  ///< item type CString

    // optional attributes

    CString m_strCount;   ///< item count CString
    CString m_strRefer;   ///< item refer CString
    CString m_strSelect;  ///< item select CString, reserved
    CString m_strId;      ///< item ID CString
};

/**
 * JAVA type
 *
 * like:
 *   <types>
 *      <type name=""demo"> -> CJavaType
 *        ...
 *      </type>
 *      ...
 *   </types>
 */

class CJavaType
{
public:

    /**
     * typedef for std::list<CJavaTypeItem*>
     */

    typedef std::list<CJavaTypeItem*> CJavaTypeItemList;

public:

    /**
     * ctor
     */

	CJavaType();

    /**
     * dtor
     */

	~CJavaType();

    /**
     * set type name
     *
     * @param strName type name CString
     */

    inline void SetName(CString& strName) { m_strName = strName; }

    /**
     * set class name
     *
     * @param strClass class name CString
     */

    inline void SetClass(CString& strClass) { m_strClass = strClass; }

    /**
     * get type name
     *
     * @return type name CString
     */

    inline CString& GetName() { return m_strName; }

    /**
     * get class name
     *
     * @return class name CString
     */

    inline CString& GetClass() { return m_strClass; }

    /**
     * insert a CJavaTypeItem
     *
     * @param poItem java item pointer
     */

    void InsertItem(CJavaTypeItem* poItem);

    /**
     * check whether has specific CJavaTypeItem
     *
     * @param strItemName item name CString
     * @return CJavaTypeItem pointer, NULL means NOT FOUND
     */

    CJavaTypeItem* HasItem(CString& strItemName);

    /**
     * get CJavaTypeItemList
     *
     * @return CJavaTypeItemList const reference
     */

    inline const CJavaTypeItemList& GetItemList() { return m_listTypeItems; }

protected:
    CString m_strName;  ///< type name CString
    CString m_strClass; ///< type class CString

    CJavaTypeItemList m_listTypeItems; ///< item list
};

/**
 * JAVA protocol
 *
 * like:
 *   <coder package="com.right.gameProto" name="GameProto" version="1"> -> CJavaProto
 *     ...
 *   </coder>
 */

class CJavaProto
{
public:

    /**
     * ctor
     */

    CJavaProto();

    /**
     * dtor
     */

    ~CJavaProto();

    /**
     * set protocol name
     *
     * @param strName protocol name CString
     */

    inline void SetName(CString& strName) { m_strName = strName; }

    /**
     * set protocol version
     *
     * @param strVersion protocol version CString
     */

    inline void SetVersion(CString& strVersion) { m_strVersion = strVersion; }

    /**
     * set protocol JAVA package name
     *
     * @param strPackageName protocol JAVA package CString
     */

    inline void SetPackageName(CString& strPackageName) { m_strPackageName = strPackageName; }

    /**
     * get protocol name
     *
     * @return protocol name CString
     */

    inline CString& GetName() { return m_strName; }

    /**
     * get protocol version
     *
     * @return protocol version CString
     */

    inline CString& GetVersion() { return m_strVersion; }

     /**
     * get protocol JAVA package name
     *
     * @return protocol JAVA package CString
     */

    inline CString& GetPackageName() { return m_strPackageName; }

protected:
    CString m_strName;         ///< protocol name CString
    CString m_strVersion;      ///< protocol version CString
    CString m_strPackageName;  ///< protocol JAVA package name CString
};

#endif // javaproto_h
