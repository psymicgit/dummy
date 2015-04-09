

#ifndef SDCOMMANDLINE_H
#define SDCOMMANDLINE_H

#include <sdtype.h>
#include "sdstring.h"
#include <vector>
#include <sddebug.h>

#define COMMAND_NAME_LENGTH 20  ///< 命令名称最大允许长度    
#define COMMAND_PARAM_LENGTH 512 ///< 命令行参数最大允许长度    

namespace SGDP{
    /**
    * @brief CSDCommandLine类
    * 底层框架命令行处理类。
    */
    class CSDCommandLine
    {
    public:
        /**
        * @brief CCommand类
        * 单个命令管理类。
        */
        class CCommand
        {
        public:
			CCommand()
			{
				memset(m_pszCommandName, 0x00, sizeof(m_pszCommandName));
				memset(m_pszCommandParam, 0x00, sizeof(m_pszCommandParam));
			}
	        /**
	        * @brief 设定命令名称
	        * @param pszName : 命令名称
	        */
            VOID SetCommand(const TCHAR* pszName)
            {
                _SDTStrcpy(m_pszCommandName,pszName);
            }

	        /**
	        * @brief 获取命令值
	        * @return 命令值
	        */
            const TCHAR* GetCommand()
            {
                return m_pszCommandName;
            }

	        /**
	        * @brief 设定命令参数
	        * @param pszName : 命令参数
	        */
            VOID SetCommandParam(const TCHAR* pszParam)
            {
                _SDTStrcpy(m_pszCommandParam,pszParam);
            }

            /**
	        * @brief 获取命令参数
	        * @return 命令参数
	        */
            const TCHAR* GetCommandParam()
            {
                return m_pszCommandParam;
            }
        private:
            TCHAR m_pszCommandName[COMMAND_NAME_LENGTH];   ///< 命令名称
            TCHAR m_pszCommandParam[COMMAND_PARAM_LENGTH]; ///< 命令对应参数
        };
    public:
        /**
	    * @brief 构造函数
	    */
        CSDCommandLine();
        
        /**
	    * @brief 构造函数，内部将调用ParserCommandLine();
	    * @param pszCommandLine : 字符串
	    */
        CSDCommandLine(const TCHAR* pszCommandLine);

        /**
	    * @brief 析构函数
	    */
        ~CSDCommandLine();

	    /**
	    * @brief 设定命令行数据
	    * @param pszName : 命令行数据
	    */
        VOID SetCommandLine(const TCHAR* pszCommandLine);

        /**
	    * @brief 移动当前命令对象指针为第一个
	    * @return 第一个命令对象指针
	    */
        CCommand* GetFirstCommand();

        /**
	    * @brief 移动当前命令对象指针为下一个
	    * @return 下一个命令对象指针
	    */
        CCommand* GetNextCommand();
    protected:

        /**
	    * @brief 依据m_pszCommandLine，解析命令行
	    */
        VOID ParserCommandLine();
        const TCHAR* m_pszCommandLine;        ///< 命令行字符串指针
        std::vector<CCommand*> m_oCommandVec;  ///< 命令对象集
        INT32 m_nCurrent;                     ///< 记录当前指针位置
    };
};

#endif

