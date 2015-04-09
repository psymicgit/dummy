#include "ReadIni.h"

#include <string>
#include <iostream>
using namespace std;
char * CReadIni::Trim( char *pszStr )
{
    char *pszBegin = NULL;
    char *pszEnd = NULL;
    char *pszTmp = NULL;

    if ( pszStr == NULL )
        return NULL;

    pszBegin = pszStr;
    while( *pszBegin == ' ' || *pszBegin == '\t' )  pszBegin++;

    pszTmp = pszBegin;
    while( *pszTmp )
    {
        switch(*pszTmp)
        {
        case ' ':
        case '\t':
            if(pszEnd == NULL)
                pszEnd = pszTmp;
            break;
        default:
            if(pszEnd != NULL)
                pszEnd = NULL;
            break;
        }
        pszTmp++;
    }
    if(pszEnd == NULL)
        pszEnd = pszTmp;

    *pszEnd = '\0';

    memmove(pszStr, pszBegin, pszEnd - pszBegin + 1);

    return pszStr;
}

char * CReadIni::TruncStr( char *pszStrSrc , char *pszStrDest , int nLen)
{
    memset(pszStrDest , 0 ,  MAX_ITEM_NAME_LEN);
    strncpy(pszStrDest , pszStrSrc , nLen) ;
    return pszStrDest;
}

/* 用cext填充字符串右边 */
char * CReadIni::RightPad( char *pszStr , char cExt , int nLen)
{
    int nCurLen;

    nCurLen = strlen(pszStr);
    if(nCurLen < nLen)
    {
        while(nCurLen < nLen)
            pszStr[nCurLen++] = cExt;
        pszStr[nLen] = '\0';
    }
    return pszStr;
}


/************************************************
Check if the Section exists in the File
Exists: return 1,
else: return 0;
************************************************/
bool CReadIni::SectionCheck(const char *pszSection)
{
    char szSect[LINEMAXCHARS] = "";
    char szBuffer[LINEMAXCHARS] = "";
    if (!m_bIsInit)
    {
        if (Init() != 0)
        {
            //printf("调用函数Init()失败!\n");
            return false;
        }
    }
    if (pszSection == NULL)
    {
        //printf("传入指针为空!\n");
        return false;
    }
    sprintf(szSect,"[%s]",pszSection);

    while ( fgets(szBuffer,LINEMAXCHARS,m_fd))
    {
        Trim( szBuffer );

        /* note or empty line */
        if ( ( szBuffer[0] == '#' ) || ( szBuffer[0] == '\n' ) )
        {
            continue;
        }

        /* into the section */
        if(strstr(szBuffer,szSect) != NULL)
        {
            return true;
        }
    }

    return false;
}

/* Read from system ini file parameter */
char * CReadIni::ReadString(const char *pszSection, const char *pszIdent, char *pszItem, const char *pszDefStr )
{
    char szBuffer[LINEMAXCHARS] = "";
    char szSect[LINEMAXCHARS] = "";
    char *p = NULL;
    char szTmpBuffer[MAX_ITEM_NAME_LEN];
    int  nAct = 0;
    size_t nLen = 0;
    char *pszTmp = NULL;
    char szTmp1[LINEMAXCHARS] = "";

    if (!m_bIsInit)
    {
        if (Init() != 0)
        {
            //printf("调用函数Init()失败!\n");
            return NULL;
        }
    }
    nAct = 0;
    if ( pszDefStr == NULL )
        strcpy( pszItem, "" );
    else
        strcpy( pszItem, pszDefStr );

    if(!m_fd)
        return pszItem;

    strcpy( szSect, "[" );
    strcat( szSect, pszSection );
    strcat( szSect, "]" );

    memset(szTmpBuffer , 0 , MAX_ITEM_NAME_LEN);

    fseek(m_fd,0,SEEK_SET );
    while ( fgets( szBuffer, LINEMAXCHARS, m_fd ) )
    {
        Trim( szBuffer );
        pszTmp = (char *) szBuffer;

        /* note or empty line */
        if ( ( szBuffer[0] == '#' ) || ( szBuffer[0] == '\n' ) )
            continue;

        /* into back pszSection of the pszSection */
        if ( nAct && ( strchr( szBuffer, '[' ) ) && ( strchr( szBuffer, ']' ) ) )
            break;

        /* into the pszSection */
        if ( !nAct && strstr( szBuffer, szSect ) ){
            nAct = 1;
            continue;
        }

        /* front of the pszSection */
        if ( !nAct )
            continue;

        if ( ( p = (char *) strchr( szBuffer, '=' ))  &&
            (strcmp(Trim(TruncStr(szBuffer,szTmp1,(int)(p-pszTmp))),pszIdent)==0) )
        {
            nLen = 0;
            if ( p = ( char * )strchr( szBuffer, '#' ) )
                nLen = (int)(p - szBuffer );//nLen = (int)(p - sBuffer + 1); modified by deng.yong
            nLen = ( nLen? nLen : strlen( szBuffer ));
            if(szBuffer[nLen-1] == '\n')
                szBuffer[nLen-1] = '\0';
            szBuffer[nLen] = '\0';
            p = ( char * )strchr( szBuffer, '=' ) ;
            nAct = 2 ;
            break;
        }
    }

    if ( nAct == 2 ){
        strcpy( pszItem, Trim( p + 1 ) );
    }
    else if( pszDefStr == NULL )
        return( NULL );

    //增加对环境变量的判断,如果字符串带${}类型,以环境变量值替换 add by kuaixp
    std::string sTmpItem=std::string(pszItem);
    int nBeginPos=sTmpItem.find("${");
    int nEndPos=sTmpItem.rfind("}");
    if(nBeginPos!=-1&&nEndPos!=-1&&nEndPos>nBeginPos)
    {
        char * pszEnvValue=NULL;

        pszEnvValue=getenv(sTmpItem.substr(nBeginPos+2,nEndPos-nBeginPos-2).c_str());
        if(pszEnvValue!=NULL)
        {
            sTmpItem.replace(nBeginPos,nEndPos-nBeginPos+1,std::string(pszEnvValue));
            strcpy(pszItem,sTmpItem.c_str());
        }

    }

    return pszItem ;
}


int CReadIni::ReadInteger(const char *pszSection, const char *pszIdent, const int nDefint)
{
    char szDefStr[LINEMAXCHARS] = "";
    char szItem[LINEMAXCHARS] = "";
    char *pszTmp = NULL;

    sprintf(szDefStr, "%d", nDefint);
    pszTmp = ReadString(pszSection, pszIdent, szItem, szDefStr);
    if ( pszTmp != NULL )
    {
        return atoi(szItem);
    }
    return nDefint;
}


/* added by yugj at 2008-11-19. reason: U34690 */
int CReadIni::WriteString(const char *pszSection, const char *pszIdent, char *pszItem)
{
    char sBuffer[LINEMAXCHARS], sSect[LINEMAXCHARS], *p , sTmpBuffer[MAX_ITEM_NAME_LEN];
    bool bInsert = false;
    int  nSection = 0;
    int iIdentLen = 0;
    int ifoffsetpre = 0;
    int ifoffsetcur = 0;
    size_t nOrigLen = 0;
    size_t nWriteLen = 0;
    size_t nValLen = 0;
    char *pTmp = NULL;
    char sTmp1[LINEMAXCHARS];
    char sWriteLine[LINEMAXCHARS];
    char sOrigBuf[LINEMAXCHARS];
    static char sFileBuf[FILEMAXCHARS];
    size_t nfLen = 0;

    if (!m_bIsInit)
    {
        if (Init() != 0)
        {
            //printf("调用函数Init()失败!\n");
            return -1;
        }
    }
    iIdentLen = strlen(pszIdent);
    if(iIdentLen <= 0)
        return -1;

    if(!m_fd)
        return -1;

    strcpy( sSect, "[" );
    strcat( sSect, pszSection );
    strcat( sSect, "]" );
    memset(sTmpBuffer , 0 , MAX_ITEM_NAME_LEN);

    fseek(m_fd,0,SEEK_SET );
    while ( fgets( sBuffer, LINEMAXCHARS, m_fd ) )
    {
        /* save preline position */
        ifoffsetpre = ifoffsetcur;
        /* get current position */
        ifoffsetcur = ftell(m_fd);
        strcpy(sOrigBuf,sBuffer);

        Trim( sBuffer );
        pTmp = (char *) sBuffer;

        /* note or empty line */
        if ( ( sBuffer[0] == '#' ) || ( sBuffer[0] == '\n' ) )
            continue;

        /* into back section of the section */
        if ( nSection && ( strchr( sBuffer, '[' ) ) && ( strchr( sBuffer, ']' ) ) )
        {
            nSection = 2;
            break;
        }

        /* into the section */
        if ( !nSection && strstr( sBuffer, sSect ) )
        {
            nSection = 1;
            continue;
        }

        /* front of the section */
        if ( !nSection )
        {
            continue;
        }


        if(strncmp(sBuffer, pszIdent, iIdentLen) == 0 &&
            (p = (char *) strchr( sBuffer, '=' )) &&
            (sBuffer[iIdentLen] == ' ' || sBuffer[iIdentLen] == '=' || sBuffer[iIdentLen] == '\t'))
        {//有SECTION,有KEY值
            sprintf(sWriteLine, "%s = %s ", pszIdent, pszItem);
            nWriteLen = strlen( sWriteLine );
            nValLen = strlen(sOrigBuf);

            nOrigLen = 0;
            if ( p = ( char * )strchr( sOrigBuf, '#' ) )
                nOrigLen = (int)(p - sOrigBuf );
            nOrigLen = ( nOrigLen? nOrigLen : nValLen);
            if(sOrigBuf[nOrigLen-1] == '\n')
            {
                nOrigLen--;
            }
            RightPad(sWriteLine, ' ', nOrigLen);

            if(nOrigLen >= nWriteLen)
            {//原始值长度大于更新值时
                bInsert = true;
                strcat(sWriteLine, p);
                sWriteLine[nValLen-1] = '\n';
                sWriteLine[nValLen] = '\0';
            }
            else
            {
                strcat(sWriteLine, "    ");
                strcat(sWriteLine, p);
                sWriteLine[strlen(sWriteLine) - 1] = '\n';
            }

            nSection = 100;
            break;
        }

    }

    switch(nSection)
    {
    case 0: //无SECTION信息
        sprintf(sWriteLine, "\n%s\n%s = %s    \n\n", sSect, pszIdent, pszItem);
        nValLen = 0;
        ifoffsetpre = ifoffsetcur;
        break;
    case 1: //有SECTION,无KEY值,无下个SECTION
        sprintf(sWriteLine, "\n%s = %s    \n\n", pszIdent, pszItem);
        nValLen = 0;
        ifoffsetpre = ifoffsetcur;
        break;
    case 2: //有SECTION,无KEY值,有下个SECTION
        sprintf(sWriteLine, "%s = %s    \n\n", pszIdent, pszItem);
        nValLen = 0;
        break;
    default:
        break;
    }


    if(bInsert)
    {//更新一行
        fclose(m_fd);
        m_fd = fopen (m_strFileName.c_str(), "r+");
        if (!m_fd)
        {
            m_fd = NULL;
            //throw TException("Open [%s] Error.",m_strFileName.c_str());
            sprintf(m_pszLastError,"Open [%s] Error.",m_strFileName.c_str());
            return -1;
        }
        fseek(m_fd,ifoffsetpre,SEEK_SET);
        fwrite(sWriteLine, 1, strlen(sWriteLine), m_fd);
        fclose(m_fd);
    }
    else
    {//更新整个文件
        fseek(m_fd,0,SEEK_SET);
        /* 取之前的信息*/
        fread(sFileBuf, 1, ifoffsetpre, m_fd);
        sFileBuf[ifoffsetpre] = '\0';
        //删除空行
        if (ifoffsetpre > 2 && strstr(sFileBuf + ifoffsetpre - 2, "\n\n") != NULL)
        {
            sFileBuf[--ifoffsetpre] = '\0';
        }

        strcat(sFileBuf, sWriteLine);
        /* 取之后的信息，对有KEY信息的跳过该行*/
        if(nValLen != 0)
        {
            fseek(m_fd, nValLen, SEEK_CUR);
        }

        nWriteLen = strlen(sWriteLine);
        nfLen = ifoffsetpre + nWriteLen;
        nfLen += fread(sFileBuf + nfLen, 1, FILEMAXCHARS - nfLen, m_fd);
        sFileBuf[nfLen] = '\0';
        fclose(m_fd);
        /* 更新文件*/
        m_fd = fopen (m_strFileName.c_str(), "w+");
        if (!m_fd)
        {
            m_fd = NULL;
            //throw TException("Open [%s] Error.",m_strFileName.c_str());
            sprintf(m_pszLastError,"Open [%s] Error.",m_strFileName.c_str());
            return -1;
        }
        fwrite(sFileBuf, 1, nfLen, m_fd);
        fclose(m_fd);
    }

    m_fd = fopen (m_strFileName.c_str(), "r");
    if (!m_fd)
    {
        m_fd = NULL;
        sprintf(m_pszLastError,"Open [%s] Error.",m_strFileName.c_str());
        return -1;
    }

    return 0 ;
}

char* CReadIni::GetLastError()
{
    return m_pszLastError;
}

int CReadIni::Init()
{
    m_strFileName = m_pszFileName;
    m_pszLastError = new char[512];
    if (m_pszLastError == NULL)
    {
        strcpy(m_pszLastError,"new char Error");
        //printf("[%s]\n",m_pszLastError);
        return -1;
    }
    if ( (m_pszFileName==NULL) || strstr(m_pszFileName,"App.config") != NULL )
    {

        m_strFileName=getenv("HBCONFIGDIR");
        if (strlen(m_strFileName.c_str()) == 0)
        {
            delete [] m_pszLastError;
            m_pszLastError = NULL;
            strcpy(m_pszLastError,"getenv HBCONFIGDIR Error");
            //printf("[%s]\n",m_pszLastError);
            //throw std::string(m_pszLastError);
            return -1;
        }

        int nLen = m_strFileName.length();
        if(m_strFileName[nLen - 1] != '/')
        {
            m_strFileName.append("/App.config");
        }
        else
        {
            m_strFileName.append("App.config");
        }
    }
    //printf("[%s]\n", m_strFileName.c_str());
    m_fd = fopen (m_strFileName.c_str(), "r");
    if (m_fd == NULL)
    {
        m_fd = NULL;
        delete [] m_pszLastError;
        m_pszLastError = NULL;
        sprintf(m_pszLastError,"Open [%s] Error.",m_strFileName.c_str());
        //printf("[%s]\n",m_pszLastError);
        //throw std::string(m_pszLastError);
        return -1;
    }
    m_bIsInit = true;
    return 0;
}

CReadIni::CReadIni(const char *pszFileName)
{
    if (pszFileName != NULL || pszFileName[0] == '\0')
    {
        m_pszFileName = new char[strlen(pszFileName)+1];
        if (m_pszFileName == NULL)
        {
            throw(string("申请空间失败\n"));
        }
        strcpy(m_pszFileName,pszFileName);
    }
    else
    {
        throw(string("传入指针为空\n"));
    }
    m_bIsInit = false;
    m_fd = NULL;
    m_pszLastError = NULL;
    return;
}

CReadIni::~CReadIni()
{
    if(m_fd != NULL)
    {
        fclose(m_fd);
    }
    m_fd = NULL;
    if (m_pszLastError != NULL)
    {
        delete[] m_pszLastError;
        m_pszLastError = NULL;
    }
    if (m_pszFileName != NULL)
    {
        delete[] m_pszFileName;
    }
    m_pszFileName = NULL;
    return;
}




