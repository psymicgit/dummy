
#ifndef _CLIDATA_H_
#define _CLIDATA_H_

#include "clisession.h"

class CPacketRobotDlg;

/**
 * @brief 全局数据
 *
 * 存放其他文件和类访问公用对象
 */
class CGlobe
{
public:
    static CPacketRobotDlg* m_poDlg;        ///< 对话框
    static CCliSession*    m_poSeesion;    ///< 客户端Session
};

#endif // #ifndef _CLIDATA_H_

