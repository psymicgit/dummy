#include "xdsapi.h"

#include <stdio.h>
#include <string.h>
#include "XDSliteAPI.h"
#include "xdsentry.h"

#define DEFINE_DSD

#define XDS_ELEMENT_TYPE 0x103

static xdsentry g_xdsentry;

namespace innerutil
{
    char* err()
    {
        static char buf[2048];
        strerror_s(buf, errno);
        // strerror(errno)

        return buf;
    }

    // 获取文件大小
    int file_len(FILE *file)
    {
        fseek (file, 0 , SEEK_END);  
        int len = ftell (file);
        rewind (file);

        return len;
    }
}

using namespace innerutil;

namespace xdsapi
{
    xdsentry* loadxds(char *xds, char *dsd)
    {
        FILE *xdsfile = NULL;
        if(fopen_s(&xdsfile, xds, "rb"))
        {
            fprintf(stderr, "Cannot open '%s' - %s\n", xds, err());
            return NULL;
        }

        FILE *dsdfile = NULL;
        if(fopen_s(&dsdfile, "powerups.dsd", "rb"))
        {
            fprintf(stderr, "Cannot open '%s' - %s\n", dsd, err());
            return NULL;
        }

        // 获取文件大小
        int dsdlen = file_len(dsdfile);  

        static unsigned char buf[10240];

        /* 将文件内容一次性全部拷贝到buffer中 */  
        if (fread (buf, 1, dsdlen, dsdfile) != dsdlen)  
        {  
            fprintf(stderr, "Reading '%s' error: %s\n", dsd, err());
            return NULL;
        }

        g_xdsentry.m_elemcnt  = 0;
        g_xdsentry.m_elements = NULL;

        struct xdsHandle *h = xdsInit(xdsfile, "Powerups", buf);
        if(NULL == h)
        {
            return NULL;
        }

        while(xdsReadRecord(h))
            ;
        xdsFini(h);
        fclose(xdsfile);

        if(g_xdsentry.isempty())
        {
            return NULL;
        }

        return &g_xdsentry;
    }
}

void XDS_PROCESSNODE(unsigned short nodeType, void *nodeData, unsigned long nodeSize)
{
    switch(nodeType)
    {
    case XDS_ELEMENT_TYPE:
        printf("ELEMENT XDS_Powerups_g_Powerups\n");
        // powerup data -- save it
        g_xdsentry.m_elements = nodeData;
        g_xdsentry.m_elemcnt = nodeSize;
        break;
    }
}