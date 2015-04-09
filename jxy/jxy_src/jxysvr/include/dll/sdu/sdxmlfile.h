/******************************************************************************



******************************************************************************/

#ifndef SDXMLFILE_H
#define SDXMLFILE_H

#include "tinyxml.h"
#include "sdstring.h"
#include <sddebug.h>

namespace SGDP
{
    /**
    * @brief xml节点类，用于记录xml文件中读到的节点
    */
    class CSDXMLNode
    {
        /**
        * @brief 记录xml文件中节点属性的值
        */
        class CXMLValue
        {
        public:

			/**
			* @brief 构造函数，初始化节点属性的值
			*/
            CXMLValue();

			/**
			* @brief 构造函数
			* @param pszValue : 要设置的字符串值
			*/
            CXMLValue(const CHAR* pszValue);

			/**
			* @brief 拷贝构造函数
			* @param v : CXMLValue对象的引用
			*/
            CXMLValue(const CXMLValue& v);

            /**
            * @brief 以下根据属性值的类型，将字符串转换为不同类型的属性值
            * @param pszString : 字符串类型 
            * @return 返回和参数对应类型的值
            */
            std::string AsString(const CHAR* pszString = "");

			/**
            * @brief 以下根据属性值的类型，将字符串转换为不同类型的属性值
            * @param nInteger : INT32类型的值 
            * @return 返回和参数对应类型的值
            */
            INT32 AsInteger(INT32 nInteger = 0);

			/**
            * @brief 以下根据属性值的类型，将字符串转换为不同类型的属性值
            * @param fFloat : FLOAT类型的值 
            * @return 返回和参数对应类型的值
            */
            FLOAT AsFloat(FLOAT fFloat = 0.0);

			/**
            * @brief 以下根据属性值的类型，将字符串转换为不同类型的属性值
            * @param bBoolean : BOOL类型的值 
            * @return 返回和参数对应类型的值
            */
            BOOL  AsBoolean(BOOL bBoolean = FALSE);

			/**
            * @brief 以下根据属性值的类型，将字符串转换为不同类型的属性值
            * @param dDouble : DOUBLE类型的值 
            * @return 返回和参数对应类型的值
            */
            DOUBLE AsDouble(DOUBLE dDouble = 0.00);
        protected:
            const CHAR* m_pszValue;
        };
    public:

		/**
		* @brief 构造函数，初始化xml节点元素
		*/
        CSDXMLNode();

		/**
		* @brief 拷贝构造函数
		* @param xmlNode : CSDXMLNode对象的引用
		*/
        CSDXMLNode(const CSDXMLNode& xmlNode);

        /**
        * @brief 重载[]操作符，根据节点名得到节点（可以相对路径的形式）
        * @param pszNodeName : 节点的名字
        * @return xml节点
        */
        CSDXMLNode operator [] (const CHAR* pszNodeName);

        /**
        * @brief 重载()操作符，得到当前节点某一属性的值
        * @param pszAttrName : 属性名字
        * @return CXMLValue对象，即属性的值
        */
        CXMLValue operator ()(const CHAR* pszAttrName);

        /**
        * @brief 将当前节点与NULL进行比较，判断是否不等，所以用指针做参数
        * @param node : xml节点（默认参数为NULL）
        * @return TRUE/FALSE
        */
        BOOL operator != ( const CSDXMLNode* node );

		/**
        * @brief 将当前节点与NULL进行比较，判断是否相等，所以用指针做参数
        * @param node : xml节点（默认参数为NULL）
        * @return TRUE/FALSE
        */
        BOOL operator == ( const CSDXMLNode* node );

        /**
        * @brief 以下根据值的类型，输出当前节点的值（获得的字符串经过必要的类型的转换）
        * @param pszString : 字符串
        * @return 对应类型的值
        */
        std::string AsString(const CHAR* pszString = "");

		/**
        * @brief 以下根据值的类型，输出当前节点的值（获得的字符串经过必要的类型的转换）
        * @param nInteger : INT32类型的值
        * @return 对应类型的值
        */
        INT32 AsInteger(INT32 nInteger = 1);

		/**
        * @brief 以下根据值的类型，输出当前节点的值（获得的字符串经过必要的类型的转换）
        * @param fFloat : FLOAT类型的值
        * @return 对应类型的值
        */
        FLOAT AsFloat(FLOAT fFloat = 1.0);

		/**
        * @brief 以下根据值的类型，输出当前节点的值（获得的字符串经过必要的类型的转换）
        * @param bBoolean : BOOL类型的值
        * @return 对应类型的值
        */
        BOOL  AsBoolean(BOOL bBoolean = FALSE);

		/**
        * @brief 以下根据值的类型，输出当前节点的值（获得的字符串经过必要的类型的转换）
        * @param dDouble : DOUBLE类型的值
        * @return 对应类型的值
        */
        DOUBLE AsDouble(DOUBLE dDouble = 1.00);
	
	/**
	* @brief 获取当前节点的第一个兄弟节点
	* @return xml节点
	*/
        CSDXMLNode Sibling();

	/**
	* @brief 根据节点名字获取当前节点的兄弟节点
	* @param szNext : 当前节点的兄弟节点名字
	* @return xml节点
	*/
        CSDXMLNode Sibling(const CHAR* szNext);

        /**
        * @brief 设置xml元素的值
        * @param tempElement : xml元素
        * @return VOID
        */
        VOID SetElement(TiXmlElement* tempElement)
        {
            m_pElement = tempElement;
        }

        /**
        * @brief 获取xml元素的值
        * @param tempElement : xml元素
        * @return VOID
        */
        TiXmlElement* GetElement()
        {
            return m_pElement;
        }
    private:
        TiXmlElement*   m_pElement;
    };

    /**
    * @brief xml文件类，从xml节点类派生
    */
    class CSDXMLFile:public CSDXMLNode
    {
    public:

		/**
		* @brief 构造函数，初始化xml文档对象
		*/
        CSDXMLFile();

		/**
		* @brief 析构函数，释放xml文档对象
		*/
        ~CSDXMLFile();

		/**
		* @brief 构造函数
		* @param pszXmlFile : xml文件名
		*/
        CSDXMLFile(const CHAR* pszXmlFile);

        /**
        * @brief 加载xml文件
        * @param pszXmlFile : xml文件名
        * @return TRUE:加载成功 FALSE:加载失败
        */
        BOOL Load(const CHAR* pszXmlFile);

        /**
        * @brief 加载xml数据
        * @param pszData : xml数据
        * @return TRUE:加载成功 FALSE:加载失败
        */
		BOOL LoadData(const CHAR* pszData, const UINT32 dwDataLen);

        /**
        * @brief 销毁xml文档对象
        * @return VOID
        */
        VOID UnLoad();

        /**
        * @brief 获取xml文件的根节点
        * @return xml节点对象
        */
        CSDXMLNode GetRootNode();

        /**
        * @brief 判断xml文档对象是否有效
        * @return TRUE:有效 FALSE:无效
        */
        BOOL       IsValid();
    protected:
        TiXmlDocument*	m_pDocument;
        
    };
}
#endif

