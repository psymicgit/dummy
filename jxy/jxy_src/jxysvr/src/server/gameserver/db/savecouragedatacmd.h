

#ifndef _SAVECOURAGEDATACMD_H_
#define _SAVECOURAGEDATACMD_H_

#include "basedbccmd.h"
#include <protocommondata.h>

using namespace  SGDP;

/**
* @brief DBCommand,异步处理的DBCommand
*
*
*/
class CSaveCourageDataCMD : public CBaseDBCMD
{
public:
    CSaveCourageDataCMD();
    ~CSaveCourageDataCMD();

    /**
    * @brief 异步执行SQL命令
    * @poDBConn 异步执行SQL命令的Connection连接
    * @return VOID
    */
    VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection* poDBConn);

    /**
    * @brief 同步执行SQL命令相关的逻辑部分
    * @return VOID
    */
    VOID SDAPI OnExecuted();


    virtual VOID SetUserData(VOID* pUserData);

public:
    virtual EDBCmdType GetDBCmdType()
    {
        return EDBCT_SAVE_COURAGE_DATA ;
    }

protected:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    BOOL	SaveData(ISDDBConnection* poDBConn);//保存



private:
    DT_COURAGE_GROUP_DATA_LST m_stDT_COURAGE_GROUP_DATA_LST;

};

#endif //#ifndef _GETPLAYERDATACMD_H_

