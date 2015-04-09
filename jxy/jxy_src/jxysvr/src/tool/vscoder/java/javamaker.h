#ifndef javamaker_h
#define javamaker_h

#include <sstream> // for std::stringstream

#include "javaloader.h"

/**
 * generate JAVA protocol related files
 */

class CJavaMaker
{
public:

    /**
     * ctor
     */

    CJavaMaker() {}

    /**
     * dtor
     */

    ~CJavaMaker() {}

    /**
     * set protocol files' output directory, reserved
     *
     * @param strOutputPath output directory CString
     */

	void SetOutputPath(CString& strOutputPath){ m_strOutputPath = strOutputPath; }

    /**
     * the real generator
     *
     * @param poProto CJavaLoader pointer
     * @param strFilePath output directory
     * @return true on SUCCESS, false on FAILURE
     */

	bool Make(CJavaLoader* poProto, CString& strFilePath);

    /**
     * get error
     *
     * @return error CString
     */

	inline CString GetErrString(){ return m_strError; }

protected:

    /**
     * generate CProtoBaseObj.java
     *
     * @param strFilePath output directory
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeBaseClassFile(CString& strFilePath);

    /**
     * generate class file for each type
     *
     * @param strFilePath output directory
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeClassFiles(CString& strFilePath);

    //
    // for protocol decoder/encoder class file
    //

    /**
     * generate protocol decoder/encoder class file
     *
     * @param strFilePath output directory
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeCodec(CString& strFilePath);

    /**
     * generate imported files
     *
     * @param output std::stringstream output
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeIncludes(std::stringstream& output);

    /**
     * generate defines
     *
     * @param output std::stringstream output
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeDefines(std::stringstream& output);

    /**
     * generate type's decoder
     *
     * @param output std::stringstream output
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeTypeDecoder(std::stringstream& output);

    /**
     * generate type's encoder
     *
     * @param output std::stringstream output
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeTypeEncoder(std::stringstream& output);

    /**
     * generate decoder for types
     *
     * @param output std::stringstream output
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeCodecDecoder(std::stringstream& output);

    /**
     * generate encoder for types
     *
     * @param output std::stringstream output
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeCodecEncoder(std::stringstream& output);

    /**
     * generate decoder for each type item
     *
     * @param output std::stringstream output
     * @param item CJavaTypeItem pointer
     * @param protoType the type name in protocol
     * @param javaType the type name in JAVA
     * @param indent source line indent
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeCodecDecoderType(std::stringstream& output, CJavaTypeItem* item,
        CString& protoType, CString& javaType, const char* indent = "\t\t\t\t");

    /**
     * generate encoder for each type item
     *
     * @param output std::stringstream output
     * @param item CJavaTypeItem pointer
     * @param protoType the type name in protocol
     * @param javaType the type name in JAVA
     * @param indent source line indent
     * @return true on SUCCESS, false on FAILURE
     */

    bool _MakeCodecEncoderType(std::stringstream& output, CJavaTypeItem* item,
        CString& protoType, CString& javaType, const char* indent = "\t\t\t\t");

protected:
	CJavaLoader* m_poProto;        ///< CJavaLoader pointer
	CString      m_strError;       ///< error CString
	CString      m_strOutputPath;  ///< output directory
};

#endif // javamaker_h
