#include "stdafx.h"

#include <cstdio>

#include "javamaker.h"

#ifdef _DEBUG 
#define new DEBUG_NEW 
#endif

bool CJavaMaker::Make(CJavaLoader* poProto, CString& strFilePath)
{
    m_poProto = poProto;

    if (!_MakeBaseClassFile(strFilePath))
    {
        return false;
    }

    if (!_MakeClassFiles(strFilePath))
    {
        return false;
    }

    if (!_MakeCodec(strFilePath))
    {
        return false;
    }

    return true;
}

bool CJavaMaker::_MakeDefines(std::stringstream& output)
{
    CJavaLoader::CJavaDefineVector::const_iterator defineGuard =
        m_poProto->GetDefineVector().begin();

    // make defines

    for (; defineGuard != m_poProto->GetDefineVector().end(); defineGuard++)
    {
        if (!IsNumber((*defineGuard)->GetValue()))
        {
            m_strError.Format("CJavaMaker Error : coder -> define value must be integer!");
            return false;
        }

        output << "\tpublic static final int "
               << (*defineGuard)->GetName().GetString()
               << " = "
               << (*defineGuard)->GetValue().GetString()
               << ";\n";
    }

    output << "\n";

    return true;
}

bool CJavaMaker::_MakeTypeEncoder(std::stringstream& output)
{
    CJavaLoader::CJavaTypeVector::const_iterator typeGuard =
        m_poProto->GetTypeVector().begin();

    // make encoder for types

    for (; typeGuard != m_poProto->GetTypeVector().end(); typeGuard++)
    {
        output << "\tpublic static byte[] Encode"
               << (*typeGuard)->GetName().GetString()
               << "(CNetData codec, "
               << (*typeGuard)->GetName().GetString()
               << " obj) {\n";

        CJavaType::CJavaTypeItemList::const_iterator itemGuard =
            (*typeGuard)->GetItemList().begin();

        // make encoder for items

        for (; itemGuard != (*typeGuard)->GetItemList().end(); itemGuard++)
        {
            if ("BYTE" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecEncoderType(output, *itemGuard, CString("Byte"), CString("byte"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("WORD" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecEncoderType(output, *itemGuard, CString("Word"), CString("short"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("DWORD" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecEncoderType(output, *itemGuard, CString("Dword"), CString("int"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("INT" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecEncoderType(output, *itemGuard, CString("Int"), CString("int"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("UINT64" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecEncoderType(output, *itemGuard, CString("Uint64"), CString("long"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("STRING" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecEncoderType(output, *itemGuard, CString("String"), CString("String"), "\t\t"))
                {
                    return false;
                }
            }
            else
            {
                // TODO: get type, use a vector to save union name
                // TODO: struct or union recursively

                // has 'count' attrubute
                if (!(*itemGuard)->GetCount().IsEmpty())
                {
                    // has 'refer' attribute
                    if (!(*itemGuard)->GetRefer().IsEmpty())
                    {
                        output << "\t\tfor (int i = 0; i < obj."
                               << (*itemGuard)->GetRefer().GetString()
                               << "; i++) {\n";

                        output << "\t\t\tEncode"
                               << (*itemGuard)->GetType().GetString()
                               << "(codec, obj."
                               << (*itemGuard)->GetName().GetString()
                               << "[i]);\n";

                        output << "\t\t}\n";
                    }
                    else
                    {
                        output << "\t\tfor (int i = 0; i < "
                               << (*itemGuard)->GetCount().GetString()
                               << "; i++) {\n";

                        output << "\t\t\tEncode"
                               << (*itemGuard)->GetType().GetString()
                               << "(codec, obj."
                               << (*itemGuard)->GetName().GetString()
                               << "[i]);\n";

                        output << "\t\t}\n";
                    }
                }
                else
                {
                    output << "\t\tEncode"
                               << (*itemGuard)->GetType().GetString()
                               << "(codec, obj."
                               << (*itemGuard)->GetName().GetString()
                               << ");\n";
                }
            }
        }

        output << "\t\treturn codec.GetData();\n"
               << "\t}\n\n";
    }

    return true;
}

bool CJavaMaker::_MakeTypeDecoder(std::stringstream& output)
{
    CJavaLoader::CJavaTypeVector::const_iterator typeGuard =
        m_poProto->GetTypeVector().begin();

    // make decoder for types

    for (; typeGuard != m_poProto->GetTypeVector().end(); typeGuard++)
    {
        output << "\tpublic static "
               << (*typeGuard)->GetName().GetString()
               << " Decode"
               << (*typeGuard)->GetName().GetString()
               << "(CNetData codec) {\n"
               << "\t\t"
               << (*typeGuard)->GetName().GetString()
               << " obj = new "
               << (*typeGuard)->GetName()
               << "();\n";

        CJavaType::CJavaTypeItemList::const_iterator itemGuard =
            (*typeGuard)->GetItemList().begin();

        // make decoder for items

        for (; itemGuard != (*typeGuard)->GetItemList().end(); itemGuard++)
        {
            if ("BYTE" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecDecoderType(output, *itemGuard, CString("Byte"), CString("byte"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("WORD" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecDecoderType(output, *itemGuard, CString("Word"), CString("short"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("DWORD" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecDecoderType(output, *itemGuard, CString("Dword"), CString("int"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("INT" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecDecoderType(output, *itemGuard, CString("Int"), CString("int"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("UINT64" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecDecoderType(output, *itemGuard, CString("Uint64"), CString("long"), "\t\t"))
                {
                    return false;
                }
            }
            else if ("STRING" == (*itemGuard)->GetType())
            {
                if (!_MakeCodecDecoderType(output, *itemGuard, CString("String"), CString("String"), "\t\t"))
                {
                    return false;
                }
            }
            else
            {
                // TODO: get type, use a vector to save union name
                // TODO: struct or union recursively
                // TODO: now, no requirment for java union support

                if (!(*itemGuard)->GetCount().IsEmpty())
                {
                    output << "\t\tobj."
                           << (*itemGuard)->GetName().GetString()
                           << " = new "
                           << (*itemGuard)->GetType().GetString()
                           << "[" << (*itemGuard)->GetCount().GetString() << "];\n";

                    if (!(*itemGuard)->GetRefer().IsEmpty())
                    {
                        output << "\t\tfor (int i = 0; i < obj."
                               << (*itemGuard)->GetRefer().GetString()
                               << "; i++) {\n";

                        output << "\t\t\tobj."
                               << (*itemGuard)->GetName().GetString()
                               << "[i] = "
                               << "Decode"
                               << (*itemGuard)->GetType().GetString()
                               << "(codec);\n";

                        output << "\t\t}\n";
                    }
                    else
                    {
                        output << "\t\tfor (int i = 0; i < "
                               << (*itemGuard)->GetCount().GetString()
                               << "; i++) {\n";

                        output << "\t\t\tobj."
                               << (*itemGuard)->GetName().GetString()
                               << "[i] = "
                               << "Decode"
                               << (*itemGuard)->GetType().GetString()
                               << "(codec);\n";

                        output << "\t\t}\n";
                    }
                }
                else
                {
                    output << "\t\tobj."
                           << (*itemGuard)->GetName().GetString()
                           << " = "
                           << "Decode"
                           << (*itemGuard)->GetType().GetString()
                           << "(codec);\n";
                }
            }
        }

        output << "\t\treturn obj;\n"
               << "\t}\n\n";
    }

    return true;
}

bool CJavaMaker::_MakeCodecEncoderType(std::stringstream& output,
    CJavaTypeItem* item, CString& protoType, CString& javaType, const char* indent)
{
    
    // make encoder for basic type

    if (item->GetCount().IsEmpty())
    {
        output << indent
               << "codec.Add" << protoType.GetString()
               << "(obj."
               << item->GetName().GetString()
               << ");\n";
    }
    else
    {
        if (item->GetType() != "STRING")
        {
            if (!item->GetRefer().IsEmpty())
            {
                output << indent
                       << "for (int i = 0; i < obj."
                       << item->GetRefer().GetString()
                       << "; i++) {\n"
                       << indent
                       << "\tcodec.Add" << protoType.GetString()
                       << "(obj."
                       << item->GetName().GetString()
                       << "[i]);\n";

                output << indent << "}\n";
            }
            else
            {
                output << indent
                       << "for (int i = 0; i < "
                       << item->GetCount().GetString()
                       << "; i++) {\n"
                       << indent
                       << "\tcodec.Add" << protoType.GetString()
                       << "(obj."
                       << item->GetName().GetString()
                       << "[i]);\n";

                output << indent << "}\n";
            }
        }
        else
        {
            if (item->GetRefer().IsEmpty())
            {
                output << indent
                       << "codec.AddString"
                       << "(obj."
                       << item->GetName().GetString()
                       << ", "
                       << "obj."
                       << item->GetName().GetString()
                       << ".getBytes().length"
                       << ");\n";
            }
            else
            {
                output << indent
                       << "codec.AddString"
                       << "(obj."
                       << item->GetName().GetString()
                       << ", " << item->GetRefer().GetString() << ");\n";
            }
        }
    }

    return true;
}

bool CJavaMaker::_MakeCodecDecoderType(std::stringstream& output,
    CJavaTypeItem* item, CString& protoType, CString& javaType, const char* indent)
{
    
    // make decoder for basic type

    if (item->GetCount().IsEmpty())
    {
        output << indent << "obj." 
               << item->GetName().GetString()
               << " = "
               << "codec.Del" << protoType.GetString() << "();\n";
    }
    else // byte array
    {
        if ("String" != protoType)
        {
            if (!item->GetRefer().IsEmpty())
            {
                output << indent << "obj."
                       << item->GetName().GetString()
                       << " = "
                       << "new " << javaType.GetString()
                       << "[" << item->GetCount().GetString() << "];\n";

                output << indent
                       << "for (int i = 0; i < obj."
                       << item->GetRefer().GetString()
                       << "; i++) {\n"
                       << indent << "\tobj." << item->GetName().GetString()
                       << "[i] = codec.Del" << protoType.GetString() << "();\n";

                output << indent << "}\n";
            }
            else
            {
                output << indent << "obj."
                       << item->GetName().GetString()
                       << " = "
                       << "new " << javaType.GetString()
                       << "[" << item->GetCount().GetString() << "];\n";

                output << indent
                       << "for (int i = 0; i < "
                       << item->GetCount().GetString()
                       << "; i++) {\n"
                       << indent << "\tobj." << item->GetName().GetString()
                       << "[i] = codec.Del" << protoType.GetString() << "();\n";

                output << indent << "}\n";
            }
        }
        else
        {
            if (!item->GetRefer().IsEmpty())
            {
                output << indent << "obj."
                       << item->GetName().GetString()
                       << " = "
                       << "codec.DelString(obj." << item->GetRefer().GetString() << ");\n";
            }
            else
            {
                output << indent << "obj."
                       << item->GetName().GetString()
                       << " = "
                       << "codec.DelString(" << item->GetCount().GetString() << ");\n";
            }
        }
    }

    return true;
}

bool CJavaMaker::_MakeCodecDecoder(std::stringstream& output)
{
    output << "\tpublic static CProtoBaseObj Decode(int msgid, byte[] ary) {\n";
    output << "\t\tCNetData codec = new CNetData(ary);\n\n";
    output << "\t\tswitch (msgid) {\n";

    CJavaLoader::CJavaMessageVector::const_iterator messageGuard =
        m_poProto->GetMessageVector().begin();

    // make invoking for types's decoder

    for (; messageGuard != m_poProto->GetMessageVector().end(); messageGuard++)
    {
        output << "\t\t\tcase CProtoBaseObj."
               << (*messageGuard)->GetName().GetString()
               << ":\n"
               << "\t\t\t\treturn Decode"
               << (*messageGuard)->GetType().GetString()
               << "(codec);\n";
    }               
    
    output << "\t\t\tdefault:\n";
    output << "\t\t\t\tbreak;\n";
    output << "\t\t};\n\n";
    output << "\t\treturn new CProtoBaseObj();\n";
    output << "\t}\n";

    return true;
}

bool CJavaMaker::_MakeCodecEncoder(std::stringstream& output)
{
    output << "\tpublic static byte[] Encode(CProtoBaseObj obj) {\n";
    output << "\t\tCNetData codec = new CNetData();\n";
    output << "\t\tswitch (obj.m_msgid) {\n";

    CJavaLoader::CJavaMessageVector::const_iterator messageGuard =
        m_poProto->GetMessageVector().begin();

    // make invoking for types's encoder

    for (; messageGuard != m_poProto->GetMessageVector().end(); messageGuard++)
    {
        output << "\t\t\tcase CProtoBaseObj."
               << (*messageGuard)->GetName().GetString()
               << ":\n"
               << "\t\t\t\treturn Encode"
               << (*messageGuard)->GetType().GetString()
               << "(codec, "
               << "(" << (*messageGuard)->GetType().GetString() << ")"
               << "obj);\n";
    }               

    output << "\t\t};\n\n";

    output << "\t\treturn null;\n";
    output << "\t}\n";

    return true;
}

bool CJavaMaker::_MakeIncludes(std::stringstream& output)
{
    CJavaLoader::CJavaIncludeVector::const_iterator includeGuard =
        m_poProto->GetIncludeVector().begin();

    output << "import com.sgdp.net.CNetData;\n";

    // make import files

    for (; includeGuard != m_poProto->GetIncludeVector().end(); includeGuard++)
    {
        output << "import "
               << (*includeGuard)->GetInclude().GetString()
               << ";\n";              
    }
    
    output << "\n";

    return true;
}

bool CJavaMaker::_MakeCodec(CString& strFilePath)
{
    std::stringstream output;

    output << "package " << m_poProto->GetProto()->GetPackageName().GetString() << ";\n\n";

    if (!_MakeIncludes(output))
    {
        return false;
    }

    output << "public class " << m_poProto->GetProto()->GetName().GetString() << " {\n";

    // TODO: make decoder/encoder for struct/union
    //       Function type: xxxxType XXXDecoder(codec)

    output << "\n// Defines\n\n";

    if (!_MakeDefines(output))
    {
        return false;
    }

    output << "\n// Decoders\n\n";

    if (!_MakeTypeDecoder(output))
    {
        return false;
    }

    output << "\n// Encoders\n\n";

    if (!_MakeTypeEncoder(output))
    {
        return false;
    }

    output << "\n// Decoders Entry\n\n";

    if (!_MakeCodecDecoder(output))
    {
        return false;
    }

    output << "\n// Encoders Entry\n\n";

    if (!_MakeCodecEncoder(output))
    {
        return false;
    }

    output << "}\n";

    std::stringstream path;
    path << strFilePath.GetString()
         << "\\"
         << m_poProto->GetProto()->GetName().GetString()
         << ".java";

    FILE* fp = ::fopen(path.str().c_str(), "w");
    if (!fp)
    {
        m_strError.Format("CJavaMaker Error : coder -> cannot open output file for writing!");
        return false;
    }

    fprintf(fp, "%s", output.str().c_str());
    ::fclose(fp);

    return true;
}

bool CJavaMaker::_MakeBaseClassFile(CString& strFilePath)
{
    std::stringstream output;

    output << "package " << m_poProto->GetProto()->GetPackageName().GetString() << ";\n\n";
    output << "public class " << "CProtoBaseObj" << " {\n";

    CJavaLoader::CJavaMessageVector::const_iterator messageGuard =
        m_poProto->GetMessageVector().begin();

    // message declarations

    for (; messageGuard != m_poProto->GetMessageVector().end(); messageGuard++)
    {
        output << "\t"
               << "public final static int "
               << (*messageGuard)->GetName().GetString()
               << " = "
               << (*messageGuard)->GetID().GetString() << ";\n";
    }

    // only member variable in CProtoBaseObj

    output << "\n\tpublic int m_msgid;\n";

    output << "}\n";

    std::stringstream path;
    path << strFilePath.GetString()
         << "\\"
         << "CProtoBaseObj.java";

    FILE* fp = ::fopen(path.str().c_str(), "w");
    if (!fp)
    {
        m_strError.Format("CJavaMaker Error : coder -> cannot open output file for writing!");
        return false;
    }

    fprintf(fp, "%s", output.str().c_str());
    ::fclose(fp);

    return true;
}

bool CJavaMaker::_MakeClassFiles(CString& strFilePath)
{
    CJavaLoader::CJavaTypeVector::const_iterator typeGuard =
        m_poProto->GetTypeVector().begin();

    // make class file for each type

    for (; typeGuard != m_poProto->GetTypeVector().end(); typeGuard++)
    {
        std::stringstream output;

        output << "package " << m_poProto->GetProto()->GetPackageName().GetString() << ";\n\n";
        output << "public class " << (*typeGuard)->GetName().GetString()
               << " extends CProtoBaseObj {\n";

        CJavaType::CJavaTypeItemList::const_iterator itemGuard =
            (*typeGuard)->GetItemList().begin();

        for (; itemGuard != (*typeGuard)->GetItemList().end(); itemGuard++)
        {
            CString strUpperType = (*itemGuard)->GetType().MakeUpper();

		    if (0 == strUpperType.Compare("BYTE"))
		    {
                output << "\tpublic byte " << (*itemGuard)->GetName().GetString();
		    }
		    else if (0 == strUpperType.Compare("WORD"))
		    {
			    output << "\tpublic short " << (*itemGuard)->GetName().GetString();
		    }
		    else if (0 == strUpperType.Compare("DWORD"))
		    {
                output << "\tpublic int " << (*itemGuard)->GetName().GetString();
		    }
		    else if (0 == strUpperType.Compare("UINT64"))
		    {
                output << "\tpublic long " << (*itemGuard)->GetName().GetString();
		    }
		    else if (0 == strUpperType.Compare("STRING"))
		    {
			    output << "\tpublic String "
                       << (*itemGuard)->GetName().GetString()
                       << ";\n";
		    }
		    else if (0 == strUpperType.Compare("INT"))
		    {
			    output << "\tpublic int " << (*itemGuard)->GetName().GetString();
		    }
            else
            {
                output << "\tpublic "
                       << strUpperType.GetString()
                       << " "
                       << (*itemGuard)->GetName().GetString();
            }

            if (0 != strUpperType.Compare("STRING"))
            {
                if (!(*itemGuard)->GetCount().IsEmpty())
                {
                    output << "[];\n";
                }
                else
                {
                    output << ";\n";
                }
            }
        }

        output << "\n";

        output << "\tpublic " << (*typeGuard)->GetName().GetString() << "() {\n";
        output << "\t\t" << "m_msgid = ";

        CJavaLoader::CJavaMessageVector::const_iterator messageGuard =
            m_poProto->GetMessageVector().begin();

        CString messageIdString;
        for (; messageGuard != m_poProto->GetMessageVector().end(); messageGuard++)
        {
            if ((*messageGuard)->GetType() == (*typeGuard)->GetName())
            {
                messageIdString = (*messageGuard)->GetName();
                break;
            }
        }

        // for unused struct

        if (messageIdString.IsEmpty())
        {
            output << "0";
        }
        else
        {
            output << "CProtoBaseObj." << messageIdString.GetString();
        }

        output << ";\n";
        output << "\t}\n";
        output << "}\n";

        std::stringstream path;
        path << strFilePath.GetString()
             << "\\"
             << (*typeGuard)->GetName()
             << ".java";

        FILE* fp = ::fopen(path.str().c_str(), "w");
        if (!fp)
        {
            m_strError.Format("CJavaMaker Error : coder -> cannot open output file for writing!");
            return false;
        }

        fprintf(fp, "%s", output.str().c_str());
        ::fclose(fp);
    }

    return true;
}

