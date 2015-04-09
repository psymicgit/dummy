#ifndef _autosqlbuf_h_
#define _autosqlbuf_h_

#include <sdtype.h>

#define MAX_SQL_LEN 5120000 //sql最大长度

class CAutoSqlBuf
{
public:
    CAutoSqlBuf();
    ~CAutoSqlBuf();
public:
    CHAR* GetSqlBuf();
private:
    CHAR* m_pszSqlBuf;
};

#endif // _autosqlbuf_h_
