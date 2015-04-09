#include "collectgasmgr.h"

#include "collectgaspropmgr.h"
#include <dll/sdframework/sdloggerimpl.h>

IMPLEMENT_SINGLETON(CCollectGasMgr)

BOOL CCollectGasMgr::Init()
{
    if(FALSE == CCollectGasPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCollectGasMgr::CreateInstance failed!"), MSG_MARK);        
        return FALSE;
    }
    if(FALSE == CCollectGasPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCollectGasMgr Init failed!"), MSG_MARK);        
        return FALSE;
    }

    return TRUE;
}

CCollectGas* CCollectGasMgr::CreateCollectGas()
{
    return m_oCollectGasPool.Alloc();
}

VOID CCollectGasMgr::RemoveCollectGas(CCollectGas *poCollectGas)
{
    if(NULL == poCollectGas)
    {
        return;
    }

    m_oCollectGasPool.Free(poCollectGas);
}

