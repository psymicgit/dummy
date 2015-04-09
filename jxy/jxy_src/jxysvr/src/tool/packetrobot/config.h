#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <sdtype.h>
#include <list>

// ---------------- 宏定义 ----------------

// 配置常规字符串长度
#define STRING_SIZE_DEFAULT 128

// ---------------- 结构体 ----------------

// 输入对象
typedef struct sInputInfo
{
	// 控件ID
	INT32 nCtrlId;
	// 标题
	CHAR szCaption[STRING_SIZE_DEFAULT];
	// 默认值
	CHAR szValue[STRING_SIZE_DEFAULT];
}InputInfo;

// 报文信息
typedef struct sMsgInfo
{
	// 标题
	CHAR szCaption[STRING_SIZE_DEFAULT];
	// 描述
	CHAR szDescribe[STRING_SIZE_DEFAULT];
	// 备注
	CHAR szNote[STRING_SIZE_DEFAULT];
	// 报文参数需要的输入对象链表
	std::list<InputInfo> lsInputs;
}MsgInfo;

// ---------------- 成  员 ----------------

// 服务器IP
extern CHAR g_acServerIP[32];
// 服务器端口
extern UINT32 g_dwServerPort;
// 心跳包频率
extern UINT32 g_dwTick;
// 报文链表
extern std::list<MsgInfo> g_lsMessages;

// ---------------- 函  数 ----------------

// 读取本地配置文件，失败返回 FALSE
BOOL LoadFromXML(CHAR* _Path);

#endif

