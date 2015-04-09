///<------------------------------------------------------------------------------
//< @file:   gvglogicmodel.h
//< @author: hongkunan
//< @date:   2014年1月14日 17:24:54
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gvglogicmodel_h_
#define _gvglogicmodel_h_

#include <sdtype.h>
#include <sdsingleton.h>

/**
*@brief 该模块封装了游戏逻辑的所有服务，其中Init方法对外部框架可见。
*外部框架模块不关心Logic部分的具体实现，只需要定时调用Init方法即可。
*/

class CGvGLogicModel
{    
    DECLARE_SINGLETON(CGvGLogicModel)

public:
    CGvGLogicModel();
    ~CGvGLogicModel();

public:
    BOOL    Init();
    BOOL	OnAtExit();//退出前的回调（保存数据等等）
    VOID    Stop();
    VOID    Destroy();

private:

};

#endif //_gvglogicmodel_h_