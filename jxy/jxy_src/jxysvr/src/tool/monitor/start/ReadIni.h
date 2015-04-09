/**
 * @file ReadIni.h
 * 
 * 读取ini文件参数功能
 * 参数文件内容格式
 * [段名]
 * [参数类型] = 参数值
 * 
 * @author 
 * @version 1.0
 * @date 
 * @bug 新建，无bug
 * @bug 单元测试，未发现bug
 * @warning 未做异常情况测试，使用时请注意入参
 */

#ifndef __READINI_H__
#define __READINI_H__

#include <string>
#include <iostream>
#include <fcntl.h>

#define LINEMAXCHARS 1000
#define MAX_ITEM_NAME_LEN 64
#define FILEMAXCHARS 40000

/**
 * @brief CReadIni
 *
 * 用于读取ini配置文件的类
 * 
 */
class CReadIni
{
public:
    /**
     * @brief 构造，要求输入pszFileName!=NULL的ini文件地址
     * 
     * @param pszFileName [in] 
     * @return 
     * @retval 
     */
    CReadIni(const char *pszFileName);
    /**
     * @brief 析构，用于关闭打开的ini文件
     * 
     * @param
     * @return 
     * @retval 
     */
    virtual ~CReadIni();
    /**
     * @brief 读取字符串
     * 
     * 用于读取ini文件中对应的段名为[pszSection]参数类型[pszIdent]对应的参数，输出到pszItem
     * 
     * @param pszSection [in] 段名
     * @param pszIdent [in] 参数类型名
     * @param pszDefStr [in] 默认值
     * @param pszItem [out] 参数值
     * @return char*
     * @retval pszItem 成功
     * @retval NULL 失败
     */
    char* ReadString (const char *pszSection, const char *pszIdent, char *pszItem, const char *pszDefStr );
    /**
     * @brief 读取数值参数
     * 
     * 先调用了ReadString读取参数的字符串值，然后再做转换。
     * 
     * @param pszSection [in] 段名
     * @param pszIdent [in] 参数类型名
     * @param nDefint [in] 默认值
     * @return int
     * @retval ini文件对应段名和参数类型的参数值
     * @retval nDefint
     */
    int ReadInteger(const char *pszSection, const char *pszIdent, const int nDefint );
    /**
     * @brief 写参数配置
     * 
     * 向ini文件中的段[pszSection]参数类型[pszIdent]写入对应值[pszItem]
     * 
     * @param pszSection [in] 段名
     * @param pszIdent [in] 参数类型名
     * @param pszItem [in] 参数值
     * @return int
     * @retval 0 成功
     * @retval 其他失败
     */
    int WriteString(const char *pszSection, const char *pszIdent, char *pszItem);
    /**
     * @brief 检查文件中是否存在该参数段
     * 
     * @param pszSection [in] 段名
     * @return bool
     * @retval true 存在
     * @retval false 不存在
     */
    bool SectionCheck(const char *pszSection);
    
    /**
     * @brief 获取错误信息
     * 
     * @param  [in] 
     * @param  [out]
     * @return char*
     * @retval m_pszLastError
     */
    char* GetLastError();
private:
    /**
     * @brief 初始化功能
     * 
     * @param  [in] 
     * @param  [out]
     * @return int
     * @retval 0 成功
     * @retval 其他失败
     */
    int Init();
    /**
     * @brief 字符串拷贝
     * 
     * 将pszStrSrc的长度nLen字符复制到pszStrDest。
     * 
     * @param pszStrSrc [in] 
     * @param nLen [in] 
     * @param pszStrDest [out]
     * @return char*
     * @retval pszStrDest
     */
    char *TruncStr( char *pszStrSrc , char *pszStrDest , int nLen);
    /**
     * @brief 格式化字符串
     * 
     * 遍历字符串，从第一个非空格和制表符开始，如果中间空格和制表符，则停止遍历，并将字符串截止返回。
     * 
     * @param pszStr [in/out] 
     * @return char*
     * @retval pszStr
     * @retval NULl
     */
    char *Trim( char *pszStr );
    /**
     * @brief 用cext填充字符串右边
     * 
     * 从字符串pszStr的右侧填充cExt字符，使字符串长度达到nLen。
     * 
     * @param pszStr [in/out] 字符串 
     * @param cExt [in] 替换字符
     * @param nLen [in] 字符串替换后的长度
     * @return char*
     * @retval pszStr
     */
    char* RightPad( char *pszStr , char cExt , int nLen);

    FILE *m_fd;///<ini文件指针

    bool m_bIsInit;///<文件是否初始化

    char* m_pszFileName;///<文件名地址指针

    std::string m_strFileName;///<文件名

    char* m_pszLastError;///<错误信息
};

#endif///<__READINI_H__

