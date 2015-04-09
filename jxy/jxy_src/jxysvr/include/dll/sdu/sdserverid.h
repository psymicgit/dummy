


#ifndef SDSERVERID_H
#define SDSERVERID_H
/**
* @file sdserverid.h

* @brief ServerID类
*
**/
#include "sdtype.h"
#include <string>
#include "sdstring.h"

// 运行体类型定义
enum eServerType {
	TYPE_BALANCE_SERVER		= 1,
	TYPE_LOGIN_SERVER,	
	TYPE_GATE_SERVER,	
	TYPE_GAME_SERVER,	
	TYPE_DB_SERVER,		
	TYPE_CENTER_SERVER,	
	TYPE_GATE_LISTENER,	
	TYPE_GAME_LISTENER,	
	TYPE_CLIENT,			
	TYPE_NOTICE_SERVER,		//通知服务器
	TYPE_GM_SERVER,			//GM命令服务器
	TYPE_MS_SERVER,			//监控服务器
	TYPE_TLC_SERVER,		//第三方认证充值服务器
	TYPE_PK_SERVER,			//跨服战服务器

	TYPE_PK_Cnt,

	TYPE_INVALID_SERVER = 0,
};

namespace SGDP
{
    /**
    * @defgroup groupgametools GAME TOOLS
    * @ingroup  SGDP
    * @{
    */

    /**
    * @brief
    * 将ServerID从数字转化为字符串
    * @param dwServerID : 数字的ServerID
    * @param dwMask : 掩码，代表字符串的ServerID的每一位占数字的ServerID的多少位
    * 如，如果mask为0x08080808，
    * 则数字的ServerID的31－24位代表字符串的ServerID的第一位part1，
    * 数字的ServerID的23－16代表字符串的ServerID的第二位part2，
    * 数字的ServerID的15－8代表字符串的ServerID的第三位part3，
    * 数字的ServerID的7－0代表字符串的ServerID的第四位part4，
    * @return 转化为字符串的ServerID
    * @remark : 字符串ServerID的格式为: part1-part2-part3-part4.
    */
    tstring  SDServerIDUtoa(UINT32 dwServerID, UINT32 dwMask = 0x08080808);

    /**
    * @brief
    * 将ServerID从字符串转化为数字
    * @param pszServerID : 字符串的ServerID
    * @param dwMask : 掩码，代表字符串的ServerID的每一位占数字的ServerID的多少位
    * 如，如果mask为0x08080808，
    * 则字符串的ServerID的第一位part1代表数字的ServerID的31－24位，
    * 字符串的ServerID的第一位part2代表数字的ServerID的23－16位，
    * 字符串的ServerID的第一位part3代表数字的ServerID的15－8位，
    * 字符串的ServerID的第一位part4代表数字的ServerID的7－0位，
    * @return 转化为数字的ServerID
    * @remark : 字符串ServerID的格式为: part1-part2-part3-part4.
    */
    UINT32  SDServerIDAtou(const CHAR *pszServerID, UINT32 dwMask = 0x08080808);

	tstring GetServerShortName(UINT8 byServerType);//获得各个服务名字


	/**
    * @brief pipe连接内容信息
    */
    struct SPipeInfo
    {
        UINT32   dwID;       ///< 对方的ServerID编号        
        TCHAR    szIP[16];   ///< 对方使用的连接IP地址 字符串型数据
        UINT32   dwIP;       ///< 对方使用的连接IP地址 INT32型数据
        UINT16   wPort;      ///< 对方使用的连接端口
        BOOL     bConnect;   ///< 连接状态，即：FALSE即断开；TRUE即连接中
        BOOL     bDirection; ///< 连接方向，即：连接方或被连接方
    };
	
	#define MAX_PIPE_CONNECT_NUM 255 //最大pipe连接
	struct SAllPipeInfo
	{
		SAllPipeInfo() { byPipeNum = 0; memset(astPipeInfo, 0, sizeof(astPipeInfo));}		
		UINT8		byPipeNum;
		SPipeInfo	astPipeInfo[MAX_PIPE_CONNECT_NUM];
	};

//TODO 增加规范

    /**
    *@brief ServerID类
    */
    class CSDServerID
    {
    public:

		/**
		* @brief 构造函数，初始化服务器ID和掩码
		* @param dwID : 服务器ID
		* @param dwMask : 掩码
		*/
        CSDServerID(UINT32 dwID = 0, UINT32 dwMask = 0x08080808);

		/**
		* @brief 析构函数
		*/
        ~CSDServerID() {}

        /**
        * @brief
        * 设置ID
        * @param pszID  : 设置的数字型的ID
        * @param dwMask : 掩码
        * @return VOID
        */
        VOID  SetID(UINT32 dwID, UINT32 dwMask = 0x08080808);

        /**
        * @brief
        * 设置ID
        * @param pszID  : 设置的字符串型的ID
        * @param dwMask : 掩码
        * @return VOID
        */
        VOID  SetID(const TCHAR *pszID, UINT32 dwMask = 0x08080808);

        /**
        * @brief
        * 设置掩码
        * @param dwMask : 设置的掩码, 注意掩码数总和不能超过sizeof(UINT32)
        * @return VOID
        */
        VOID  SetMask(UINT32 dwMask);

        /**
        * @brief
        * 获取掩码
        * @return VOID
        */
        UINT32  GetMask();

        /**
        * @brief
        * 获取AreaID，即字符串型ServerID的第一位
        * @return 获取AreaID
        */
        UINT32  GetAreaID();

        /**
        * @brief
        * 获取GroupID，即字符串型ServerID的第二位
        * @return 获取GroupID
        */
        UINT32  GetGroupID();

        /**
        * @brief
        * 获取ServerType，及字符串型ServerID的第三位
        * @return 获取ServerType
        */
        UINT32  GetServerType();

        /**
        * @brief
        * 获取ServerIndex，及字符串型ServerID的第四位
        * @return 获取ServerIndex
        */
        UINT32  GetServerIndex();

        /**
        * @brief
        * 获取字符串型的ServerID
        * @return 字符串型的ServerID
        */
        tstring  AsString();

        /**
        * @brief
        * 获取数字型的ServerID
        * @return 数字型的ServerID
        */
        UINT32  AsNumber();

    private:
        UINT32       m_dwMaskLen[4]; //mask的长度，用来移位
        UINT32       m_dwSubMask[4]; //mask的值
        UINT32       m_dwID;          // 数字ID
        UINT32       m_dwMask;        //掩码,注意掩码数总和不能超过32
        tstring		 m_strID;
    };


    /**
    * @brief
    * 将ServerID从数字转化为字符串
    * @param dwServerID : 数字的ServerID
    * @param dwMask : 掩码，代表字符串的ServerID的每一位占数字的ServerID的多少位
    * 如，如果mask为0x08080808，
    * 则数字的ServerID的31－24位代表字符串的ServerID的第一位part1，
    * 数字的ServerID的23－16代表字符串的ServerID的第二位part2，
    * 数字的ServerID的15－8代表字符串的ServerID的第三位part3，
    * 数字的ServerID的7－0代表字符串的ServerID的第四位part4，
    * @return 转化为字符串的ServerID
    * @remark : 字符串ServerID的格式为: part1-part2-part3-part4.
    */
    tstring  SDServerIDExUtoa(UINT64 dwServerID, UINT32 dwMask = 0x08080808);

    /**
    * @brief
    * 将ServerID从字符串转化为数字
    * @param pszServerID : 字符串的ServerID
    * @param dwMask : 掩码，代表字符串的ServerID的每一位占数字的ServerID的多少位
    * 如，如果mask为0x08080808，
    * 则字符串的ServerID的第一位part1代表数字的ServerID的31－24位，
    * 字符串的ServerID的第一位part2代表数字的ServerID的23－16位，
    * 字符串的ServerID的第一位part3代表数字的ServerID的15－8位，
    * 字符串的ServerID的第一位part4代表数字的ServerID的7－0位，
    * @return 转化为数字的ServerID
    * @remark : 字符串ServerID的格式为: part1-part2-part3-part4.
    */
    UINT64  SDServerIDExAtou(const CHAR* pszServerID, UINT32 dwMask = 0x08080808);

    class CSDServerIDEx
    {
    public:

		/**
		* @brief 构造函数，初始化服务器ID和掩码
		* @param dwId : 服务器ID
		* @param dwMask : 掩码
		*/
        CSDServerIDEx(UINT64 dwId = 0, UINT32 dwMask = 0x08080808);

		/**
		* @brief 析构函数
		*/
        ~CSDServerIDEx() {}

        /**
        * @brief
        * 设置ID
        * @param pszID : 设置的数字型的ID
        * @param dwMask : 掩码
        * @return VOID
        */
        VOID  SetID(UINT64 dwID, UINT32 dwMask = 0x08080808);

        /**
        * @brief
        * 设置ID
        * @param pszID : 设置的字符串型的ID
        * @param dwMask : 掩码
        * @return VOID
        */
        VOID  SetID(const TCHAR* pszID, UINT32 dwMask = 0x08080808);

        /**
        * @brief
        * 设置掩码
        * @param dwMask : 设置的掩码, 注意掩码数总和不能超过sizeof(UINT32)
        * @return VOID
        */
        VOID  SetMask(UINT32 dwMask);

        /**
        * @brief
        * 获取掩码
        * @return VOID
        */
        UINT32  GetMask();


        /**
        * @brief
        * 获取游戏中id
        * @return UINT32
        */
        UINT32  GetGameId();


        /**
        * @brief
        * 获取AreaID，即字符串型ServerID的第一位
        * @return 获取AreaID
        */
        UINT32  GetAreaId();

        /**
        * @brief
        * 获取GroupID，即字符串型ServerID的第二位
        * @return 获取GroupID
        */
        UINT32  GetGroupId();

        /**
        * @brief
        * 获取ServerType，及字符串型ServerID的第三位
        * @return 获取ServerType
        */
        UINT32  GetServerType();

        /**
        * @brief
        * 获取ServerIndex，及字符串型ServerID的第四位
        * @return 获取ServerIndex
        */
        UINT32  GetServerIndex();

        /**
        * @brief
        * 获取字符串型的ServerID
        * @return 字符串型的ServerID
        */
        tstring  AsString();

        /**
        * @brief
        * 获取数字型的ServerID
        * @return 数字型的ServerID
        */
        UINT64  AsNumber();
    private:
        UINT32        m_maskLen[4]; //mask的长度，用来移位
        UINT32        m_subMask[4]; //mask的值
        UINT32        m_mask; //服务器ID掩码
        UINT64        m_id;
        tstring		  m_strId;
    };

    /** @} */
}

#endif // 


