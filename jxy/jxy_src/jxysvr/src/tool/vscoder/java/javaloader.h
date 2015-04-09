#ifndef javaloader_h
#define javaloader_h

#include <vector>

#include "tinyxml.h"
#include "javaproto.h"

/**
 * JAVA protocol XML configuration loader & parser
 */

class CJavaLoader
{
public:

    /**
     * typedef for std::vector<CJavaInclude*>
     */

    typedef std::vector<CJavaInclude*> CJavaIncludeVector;

    /**
     * typedef for std::vector<CJavaDefine*>
     */

    typedef std::vector<CJavaDefine*> CJavaDefineVector;

    /**
     * typedef for std::vector<CJavaMessage*>
     */

    typedef std::vector<CJavaMessage*> CJavaMessageVector;

     /**
     * typedef for std::vector<CJavaType*>
     */

    typedef std::vector<CJavaType*> CJavaTypeVector;

public:

    /**
     * ctor
     */

    CJavaLoader();

    /**
     * dtor
     */

    ~CJavaLoader();

    /**
     * load XML configuration file
     *
     * @param strFilePath file absolute path CString
     * @return true on SUCCESS, false on FAILURE
     */

    bool Load(CString& strFilePath);

    /**
     * get error
     *
     * @return error CString
     */

    inline CString& GetErrString() { return m_strError; }

    /**
     * get CJavaProto pointer
     *
     * @return CJavaProto pointer
     */

    inline CJavaProto* GetProto() { return m_poProto; }

    /**
     * get CJavaIncludeVector
     *
     * @return CJavaIncludeVector const reference
     */

    inline const CJavaIncludeVector& GetIncludeVector() { return m_vecIncludes; }

    /**
     * get CJavaDefineVector
     *
     * @return CJavaDefineVector const reference
     */

    inline const CJavaDefineVector& GetDefineVector() { return m_vecDefines; }

    /**
     * get CJavaMessageVector
     *
     * @return CJavaMessageVector const reference
     */

    inline const CJavaMessageVector& GetMessageVector() { return m_vecMessages; }

     /**
     * get CJavaTypeVector
     *
     * @return CJavaTypeVector const reference
     */

    inline const CJavaTypeVector& GetTypeVector() { return m_vecTypes; }

protected:

    /**
     * internal loader
     *
     * @param strFilePath file absolute path CString
     * @return true on SUCCESS, false on FAILURE
     */

    bool _Load(CString& strFilePath);

    /**
     * clean up all allocated resources
     */

    void _Clean();

    /**
     * parse to CJavaProto
     *
     * @return true on SUCCESS, false on FAILURE
     */

    bool _LoadJavaProto();

    /**
     * parse '<include>' section
     *
     * @return true on SUCCESS, false on FAILURE
     */

    bool _LoadInclude();

    /**
     * parse '<defines>' section
     *
     * @return true on SUCCESS, false on FAILURE
     */

    bool _LoadDefines();

    /**
     * parse '<messages>' section
     *
     * @return true on SUCCESS, false on FAILURE
     */

    bool _LoadMessages();

    /**
     * parse '<types>' section
     *
     * @return true on SUCCESS, false on FAILURE
     */

    bool _LoadTypes();

    /**
     * parse '<type>' section in '<types>'
     *
     * @param pTypeElement A TinyXML element for '<type>' section
     * @return true on SUCCESS, false on FAILURE
     */

    bool _LoadAType(TiXmlElement* pTypeElement);

protected:
    CJavaProto*        m_poProto;      ///< CJavaProto pointer
    CJavaIncludeVector m_vecIncludes;  ///< include vector
    CJavaDefineVector  m_vecDefines;   ///< define vector
    CJavaMessageVector m_vecMessages;  ///< message vector
    CJavaTypeVector    m_vecTypes;     ///< type vector

    CString         m_strError;        ///< error CString
    TiXmlDocument*	m_pDocument;       ///< opened XML document
	TiXmlElement*   m_pRootElement;    ///< XML root element
};

#endif // javaloader_h
