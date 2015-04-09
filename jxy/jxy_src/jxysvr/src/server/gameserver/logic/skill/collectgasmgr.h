#ifndef _collectgasmgr_h_
#define _collectgasmgr_h_

#include "collectgas.h"

#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>

class CCollectGasMgr
{
public:
    DECLARE_SINGLETON(CCollectGasMgr)

public:
    BOOL Init();

public:
    CCollectGas* CreateCollectGas();
    VOID         RemoveCollectGas(CCollectGas*);

private:
    CSDObjectPool<CCollectGas, CSDMutex>  m_oCollectGasPool;
};

#endif // _collectgasmgr_h_
