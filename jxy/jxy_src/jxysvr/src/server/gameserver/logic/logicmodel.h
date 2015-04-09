

#ifndef _LOGIC_MODEL_H_
#define _LOGIC_MODEL_H_

#include <sdtype.h>
#include <sdsingleton.h>

/**
*@brief 该模块封装了游戏逻辑的所有服务，其中Init方法对外部框架可见。
*外部框架模块不关心Logic部分的具体实现，只需要定时调用Init方法即可。
*
*/


class CLogicModel
{    
    DECLARE_SINGLETON(CLogicModel)

public:
    CLogicModel();
    ~CLogicModel();

public:
    BOOL    Init();
    BOOL    InitGvG();
	BOOL	OnAtExit();//退出前的回调（保存数据等等）
    VOID    Destroy();

	VOID	Run();

private:

};



#endif //_LOGIC_MODEL_H_

