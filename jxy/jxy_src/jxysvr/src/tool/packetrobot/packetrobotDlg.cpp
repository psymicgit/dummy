// PacketRobotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "common.h"
#include "PacketRobotDlg.h"
#include "PacketRobot.h"
#include "config.h"
#include "sdclientlogin.h"
#include "sdmsg.h"
#include "clidata.h"
#include <sdstring.h>
#include <sdtime.h>
#include <stack>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 默认输入字符串大小
#define INPUT_DEFAULT_SIZE 128

CPacketRobotDlg::CPacketRobotDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CPacketRobotDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pISDClient = NULL ;
}

VOID CPacketRobotDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_SELECT, m_lcList);
    DDX_Control(pDX, IDC_EDIT_TEXT, m_richEdit);
    DDX_Control(pDX, IDC_TREE_STRUCT, m_Tree);
}

BEGIN_MESSAGE_MAP(CPacketRobotDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CPacketRobotDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_SEND, &CPacketRobotDlg::OnBnClickedButtonSend)
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CPacketRobotDlg::OnBnClickedButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_CUT, &CPacketRobotDlg::OnBnClickedButtonCut)
    ON_BN_CLICKED(IDCANCEL, &CPacketRobotDlg::OnBnClickedCancel)
    ON_NOTIFY(NM_CLICK, IDC_LIST_SELECT, &CPacketRobotDlg::OnNMClickListSelect)
    ON_WM_TIMER()
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_STRUCT, &CPacketRobotDlg::OnTvnSelchangedTreeStruct)
END_MESSAGE_MAP()


// CPacketRobotDlg message handlers

SDBOOL CPacketRobotDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    CProtoCliGT* pProtoCli = CProtoCliGT::Instance();
    pProtoCli->SetMaxMsgId(200);
    pProtoCli->SetMinMsgId(1);
    pProtoCli->SetName("ProtoCli");
    if (NULL == CProtocolMgr::Instance()->AddProtocol(pProtoCli))
    {
        return FALSE;
    }

    CProtoCliZS* pProtoCliZS = CProtoCliZS::Instance();
    pProtoCliZS->SetMaxMsgId(200);
    pProtoCliZS->SetMinMsgId(1);
    pProtoCliZS->SetName("ProtoCliZS");
    if (NULL == CProtocolMgr::Instance()->AddProtocol(pProtoCliZS))
    {
        return FALSE;
    }

    m_szUserId[0] = 0;

    BOOL bReturn = LoadFromXML("robot.xml");
    CHAR czInfo[1024];
    if(FALSE == bReturn)
    {
        strcpy(czInfo, "robot.xml 解析失败。");
        PrintText(czInfo, LOG_ERROR);
    }
    else
    {
        TimeString(czInfo);
        // INT32 nRet = WriteLog(czInfo);
        INT32 nRet = 1;
        if(0 == nRet)
        {
            sprintf(czInfo, "[ robot.xml 解析完毕。日志 = \"%s\"，服务器IP = \"%s\"，端口号 = %d。]", "开启", g_acServerIP, g_dwServerPort);
        }
        else
        {
            sprintf(czInfo, "[ robot.xml 解析完毕。日志 = \"%s\"，服务器IP = \"%s\"，端口号 = %d。]", "关闭", g_acServerIP, g_dwServerPort);
        }
        PrintText(czInfo, LOG_DESCRIBE);
    }

    // 列表
    m_lcList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    FlushList();

    // 树型图
    m_Tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS);


    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CPacketRobotDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        INT32 cxIcon = GetSystemMetrics(SM_CXICON);
        INT32 cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        INT32 x = (rect.Width() - cxIcon + 1) / 2;
        INT32 y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPacketRobotDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 连接服务器
UINT32 CPacketRobotDlg::ConnectServer(const CHAR* _IP, const UINT32 _Port)
{
    if (m_pISDClient)
    {
        m_pISDClient->Release();
        m_pISDClient = NULL;
    }
    m_pISDClient = SDNetGetModule(&SDCLIENT_MODULE_VERSION);
    m_poConnector = m_pISDClient->CreateConnector(NETIO_ASYNCSELECT);
    m_poConnector->SetSession(CGlobe::m_poSeesion);
    m_poConnector->SetPacketParser(CCliPacketParser::Instance());
    m_poConnector->SetBufferSize(102400, 102400);
    m_poConnector->Connect(_IP, _Port);
    return 0;
}

// 断开连接
VOID CPacketRobotDlg::CutOffServer()
{
    if (m_pISDClient)
    {
        m_poConnector->Release();
        m_pISDClient->Release();
        m_pISDClient = NULL;
        OnDisConnect(0);
        // m_poConnector = NULL;
    }
}

VOID CPacketRobotDlg::OnConnect(INT32 nErrorCode)
{
    CHAR szText[LOG_MAX_LENGTH];
    sprintf(szText, "[ 服务器连接完毕 ！错误码 = %d。]", nErrorCode);
    PrintText(szText, LOG_DESCRIBE);
    if(0 == nErrorCode)
    {
        ::EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BUTTON_CUT), TRUE);
        ::EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BUTTON_SEND), TRUE);
        ::EnableWindow(::GetDlgItem(this->m_hWnd, IDOK), FALSE);
    }
}

VOID CPacketRobotDlg::OnDisConnect(INT32 nErrorCode)
{
    PrintText("[ 连接已断开。]", LOG_ERROR);
    ::EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BUTTON_CUT), FALSE);
    ::EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BUTTON_SEND), FALSE);
    ::EnableWindow(::GetDlgItem(this->m_hWnd, IDOK), TRUE);
    // 停止时钟
    KillTimer(TIMER_ID_MOVEVAST);
    KillTimer(TIMER_ID_HEARTBEAT);

    CGlobe::m_poSeesion->ClrKey();
}

// ---------------- 测试协议 ----------------


// 发送常规消息
VOID CPacketRobotDlg::Send_COMMON(INT32 nMsgID, VOID* pReq, const CHAR* szDescription)
{
    CHAR szTip[256] = {0};
    sprintf(szTip, "[ 刚发送(%s)请求。]", szDescription);
    PrintText(szTip, LOG_DESCRIBE);
    CSDProtocol* pProtocol = CProtocolMgr::Instance()->FindProtocol(nMsgID);
    if (pProtocol && m_pISDClient)
    {
        static CHAR acMsgData[PACKAGE_CONTENT_LENGTH] = {0};
        INT32 nEncodeRet = pProtocol->Encode(nMsgID, pReq, acMsgData, PACKAGE_CONTENT_LENGTH);
        if (nEncodeRet < 0)
        {
            CHAR szText[LOG_MAX_LENGTH] = {0};
            sprintf(szText, "[ MsgID = %d, Len = %d] encode error!", nMsgID);
            PrintText(szText, LOG_DESCRIBE);
            return ;
        }
        CGlobe::m_poSeesion->SendMsg(nMsgID, acMsgData, nEncodeRet);
    }
}


// ---------------- 辅助方法 ----------------

// 打印日志
VOID CPacketRobotDlg::PrintText(const CHAR* cpText, const LogType _LogType)
{
    if(LOG_DATA == _LogType)
    {
        // 打印树形图
        ParseLog(cpText);
    }
    else if(LOG_NOPRINT == _LogType)
    {
        // 仅打印树形图
        ParseLog(cpText);
        return;
    }
    TCHAR acTemp[LOG_MAX_LENGTH];
    _tcscpy(acTemp, _SDTLocal2T(cpText));
    _tcscat(acTemp, _T("\r\n"));
    WriteText(acTemp, _LogType);
}

// 打印日志
VOID CPacketRobotDlg::WriteText(const TCHAR* cpText, const LogType _LogType)
{
    INT32 nOldLines = 0, nNewLines = 0, nScroll = 0;
    LONG nInsertionPoint = 0;
    COLORREF wCollor;
    CHARFORMAT cf;
    switch(_LogType)
    {
    case LOG_DATA:
        wCollor = RGB(0, 0, 0);
        break;
    case LOG_DESCRIBE:
        wCollor = RGB(0, 0, 255);
        break;
    case LOG_ERROR:
        wCollor = RGB(255, 0, 0);
        break;
    }

    // Save number of lines before insertion of new text
    nOldLines = m_richEdit.GetLineCount();

    // Initialize character format structure
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_COLOR | CFM_SIZE | CFM_FACE;
    cf.dwEffects = 0;	// To disable CFE_AUTOCOLOR
    cf.crTextColor	= wCollor;
    cf.yHeight = 13 * 13;
    _tcscpy(cf.szFaceName, _T("微软雅黑"));

    // Set insertion point to end of text
    nInsertionPoint = m_richEdit.GetWindowTextLength();
    m_richEdit.SetSel(nInsertionPoint, -1);

    //  Set the character format
    m_richEdit.SetSelectionCharFormat(cf);

    // Replace selection. Because we have nothing selected, this will simply insert
    // the string at the current caret position.
    m_richEdit.ReplaceSel(cpText);

    // Get new line count
    nNewLines = m_richEdit.GetLineCount();

    // Scroll by the number of lines just inserted
    nScroll = nNewLines - nOldLines;
    m_richEdit.LineScroll(nScroll);
}

// 解析结构体日志
VOID CPacketRobotDlg::ParseLog(const CHAR* _Log)
{
    static INT32 s_nDepth = -1; // 上一个树的深度
    static std::stack<HTREEITEM> s_Tree;
    static HTREEITEM s_TreeItem = NULL;
    if(NULL == _Log) // 初始化解析函数
    {
        s_nDepth = -1;
        while(false == s_Tree.empty())s_Tree.pop();
        s_TreeItem = NULL;
        return;
    }
    // 清理空行
    const CHAR* pszNext = strstr(_Log, "\r\n");
    if(pszNext == _Log || '\0' == *_Log)
        return;
    // 分离行
    CHAR* pszLine;
    if(NULL == pszNext)
    {
        pszLine = new CHAR[strlen(_Log) + 1];
        strcpy(pszLine, _Log);
    }
    else
    {
        pszLine = new CHAR[pszNext - _Log + 1];
        memset(pszLine, 0, pszNext - _Log + 1);
        strncpy(pszLine, _Log, pszNext - _Log);
        pszNext += 2;
    }
    // 获取当前深度
    INT32 nCurDepth = 0; // 当前树的深度
    while(*(_Log + nCurDepth) == '\t')nCurDepth++;
    // 解析当前行
    CHAR* pKey = NULL, *pValue = NULL;
    CHAR* pszTemp = pszLine + nCurDepth;
    if(*pszTemp == '{' || *pszTemp == '}' || *pszTemp == '\0')
    {
        if(nCurDepth < s_nDepth)
        {
            s_Tree.pop();
            s_TreeItem = NULL;
            s_nDepth = nCurDepth;
        }
        delete[] pszLine;
        if(NULL != pszNext)
        {
            ParseLog(pszNext);
        }
        return;
    }
    else
    {
        CHAR* pszBuf = strstr(pszTemp, "=");
        pKey = new CHAR[strlen(pszTemp) + 1];
        if(NULL == pszBuf)
        {
            strcpy(pKey, pszTemp);
        }
        else
        {
            *(pszBuf++) = '\0';
            strcpy(pKey, pszTemp);
            pValue = new CHAR[strlen(pszBuf) + 1];
            strcpy(pValue, pszBuf);
            m_LsResource.push_back(pValue);
        }
        m_LsResource.push_back(pKey);
    }

    // 开始插入
    HTREEITEM hTreeItem;
    if(nCurDepth == s_nDepth)
    {
        // 插入兄弟子树
        hTreeItem = s_Tree.top();
        if(0 == nCurDepth)
            s_TreeItem = m_Tree.InsertItem(_SDTA2T(pKey), NULL, NULL);
        else
            s_TreeItem = m_Tree.InsertItem(_SDTA2T(pKey), NULL, NULL, hTreeItem);
    }
    else if(nCurDepth > s_nDepth)
    {
        hTreeItem = s_TreeItem;
        s_Tree.push(hTreeItem);
        if(0 == nCurDepth)
            s_TreeItem = m_Tree.InsertItem(_SDTA2T(pKey), NULL, NULL);
        else
            s_TreeItem = m_Tree.InsertItem(_SDTA2T(pKey), NULL, NULL, hTreeItem);
        s_nDepth = nCurDepth;
    }
    else
    {
        s_Tree.pop();
        hTreeItem = s_Tree.top();

        if(0 == nCurDepth)
            s_TreeItem = m_Tree.InsertItem(_SDTA2T(pKey), NULL, NULL);
        else
            s_TreeItem = m_Tree.InsertItem(_SDTA2T(pKey), NULL, NULL, hTreeItem);

        s_nDepth = nCurDepth;
    }
    m_Tree.SetItemData(s_TreeItem, (INT32)pValue);
    delete[] pszLine;
    // 解析下一行
    if(NULL != pszNext)
    {
        ParseLog(pszNext);
    }
}

// 刷新列表
VOID CPacketRobotDlg::FlushList()
{
    // 清空列表和标题栏
    m_lcList.DeleteAllItems();
    while(FALSE != m_lcList.DeleteColumn(0));
    // 添加标题栏
    m_lcList.InsertColumn(0, _T("协议名"), LVCFMT_LEFT, 100);
    m_lcList.InsertColumn(1, _T("报文标志"), LVCFMT_LEFT, 220);
    m_lcList.InsertColumn(2, _T("备注"), LVCFMT_LEFT, 80);
    // 添加列表
    INT32 i = 0;
    for(std::list<MsgInfo>::iterator itor = g_lsMessages.begin(); itor != g_lsMessages.end(); itor++)
    {
        m_lcList.InsertItem(i, _SDTLocal2T(itor->szCaption));
        m_lcList.SetItemText(i, 1, _SDTLocal2T(itor->szDescribe));
        m_lcList.SetItemText(i, 2, _SDTLocal2T(itor->szNote));
        i++;
    }
}

// ---------------- 辅助方法 ----------------

// 配置输入
VOID CPacketRobotDlg::AdjustInput(INT32 _Index)
{
    std::list<MsgInfo>::iterator itorMsg;
    std::list<InputInfo>::iterator itorInput;

    // 清理输入框
    ClearInput();

    itorMsg = g_lsMessages.begin();
    if(0 != _Index)
    {
        std::advance(itorMsg, _Index);
    }
    for(itorInput = itorMsg->lsInputs.begin(); itorInput != itorMsg->lsInputs.end(); itorInput++)
    {
        AdjustEdit(itorInput->nCtrlId, itorInput->szCaption, itorInput->szValue);
    }
}

// 清空输入框
VOID CPacketRobotDlg::ClearInput()
{
    for(INT32 i = 0; i < INPUT_MAX_COUNT; i++)
    {
        ::EnableWindow(::GetDlgItem(this->m_hWnd, IDC_EDIT1 + i), FALSE);
        ::SetDlgItemTextA(this->m_hWnd, IDC_EDIT1 + i, "");
        ::SetDlgItemTextA(this->m_hWnd, IDC_LABEL1 + i, "");
    }
}

// 配置输入框
VOID CPacketRobotDlg::AdjustEdit(INT32 _Index, CHAR* Caption, CHAR* Value)
{
    // Label Id
    INT32 nLabelId = _Index + IDC_LABEL1;
    // Edit Id
    INT32 nEditId = _Index + IDC_EDIT1;

    if(NULL == Caption)
    {
        ::SetDlgItemTextA(this->m_hWnd, nLabelId, "");
    }
    else
    {
        ::SetDlgItemTextA(this->m_hWnd, nLabelId, Caption);
    }
    ::EnableWindow(::GetDlgItem(this->m_hWnd, nEditId), TRUE);
    if(NULL == Value)
    {
        ::SetDlgItemTextA(this->m_hWnd, nEditId, "");
    }
    else
    {
        ::SetDlgItemTextA(this->m_hWnd, nEditId, Value);
    }
}


// ---------------- UI接口 ----------------

// 连接服务器
VOID CPacketRobotDlg::OnBnClickedOk()
{
    ConnectServer(g_acServerIP, g_dwServerPort);
}

// 断开连接
void CPacketRobotDlg::OnBnClickedButtonCut()
{
    CutOffServer();
}


// 清空消息框
VOID CPacketRobotDlg::OnBnClickedButtonClear()
{
    CHAR czLine[LOG_MAX_LENGTH];
    ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT_TEXT, czLine, LOG_MAX_LENGTH);
    // WriteLog(czLine);
    ::SetDlgItemTextA(this->m_hWnd, IDC_EDIT_TEXT, "");
    m_Tree.DeleteAllItems();
    ParseLog(NULL);
}

// 退出
VOID CPacketRobotDlg::OnBnClickedCancel()
{
    OnBnClickedButtonClear();
    if(NULL != m_pISDClient)
    {
        m_pISDClient->Release();
        m_pISDClient = NULL;
    }
    for(std::list<CHAR*>::iterator iter = m_LsResource.begin(); iter != m_LsResource.end(); iter++)
    {
        CHAR* pszTemp = *iter;
        delete[] pszTemp;
        pszTemp = NULL;
    }
    m_LsResource.clear();

    OnOK();
}

// 选中协议
VOID CPacketRobotDlg::OnNMClickListSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE *>(pNMHDR);
    // 配置输入框，方便操作
    if(-1 != pNMItemActivate->iItem)
    {
        AdjustInput(pNMItemActivate->iItem);
    }
    *pResult = 0;
}

// 选中结构体
void CPacketRobotDlg::OnTvnSelchangedTreeStruct(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
    INT32 n = m_Tree.GetItemData(hTreeItem);
    CHAR* pszData = (CHAR*)n;
    if(NULL == pszData)
        ::SetDlgItemTextA(this->m_hWnd, IDC_EDIT_INFO, "NULL");
    else
        ::SetDlgItemTextA(this->m_hWnd, IDC_EDIT_INFO, pszData);

    *pResult = 0;
}

VOID CPacketRobotDlg::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent)
    {
    case TIMER_ID_LOADFINISH:
        //SendLoadFinsh(0);
        // KillTimer(TIMER_ID_LOADFINISH);
        break;
    case TIMER_ID_HEARTBEAT:
        //SendHeartBeat();
        break;
    }
    __super::OnTimer(nIDEvent);
}


// 发送报文
VOID CPacketRobotDlg::OnBnClickedButtonSend()
{
    INT32 nIndex = 0;
    POSITION pos = m_lcList.GetFirstSelectedItemPosition();
    if(NULL != pos)
    {
        nIndex = m_lcList.GetNextSelectedItem(pos) + 1;
        switch(nIndex)
        {
            //认证
        case 1:
        {
            // 登陆
            CHAR szTemp[128] = {0};
            PKT_CLILS_LOGIN_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwCliVersion = (UINT32)(SDStrtod(szTemp, NULL) * 100);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byAuthType = atoi(szTemp);
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT3, stReq.aszDeviceID, INPUT_DEFAULT_SIZE);
            //::GetDlgItemText(this->m_hWnd, IDC_EDIT4, stReq.aszNotifyID, INPUT_DEFAULT_SIZE);
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT4, stReq.aszUserName, INPUT_DEFAULT_SIZE);
            CHAR aszUserPwd[USERNAME_LEN] = { 0 }; //密码
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT5, aszUserPwd, INPUT_DEFAULT_SIZE);
            if(1 == stReq.byAuthType)
            {
                CHAR acNetBuf[USERNAME_LEN] = {0};
                memcpy(acNetBuf + ENCRYPT_HEAD_LEN, aszUserPwd, strlen(aszUserPwd));
                UINT8* pszDecryptBuff =  (UINT8*)(acNetBuf);
                INT32 nDecryptBuffLen = strlen(aszUserPwd) + ENCRYPT_HEAD_LEN + ENCRYPT_TAIL_LEN;//添加加解密头尾
                CGlobe::m_poSeesion->GetMGEncrypt().encryptionDatas(pszDecryptBuff, nDecryptBuffLen);
                memcpy(stReq.abyUserPwd, pszDecryptBuff, nDecryptBuffLen);
                stReq.byPwdLen = nDecryptBuffLen;
            }
            else
            {
                stReq.byPwdLen = strlen(aszUserPwd);
                memcpy(stReq.abyUserPwd, aszUserPwd, strlen(aszUserPwd));
            }
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT6, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wZoneID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT7, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwPlayerID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT8, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wCareerID = atoi(szTemp);
			stReq.byAuthMethod = 1;
            // 发送报文
            Send_COMMON(CLILS_LOGIN_REQ, &stReq, "登陆");
        }
        break;
        //进入游戏
        case 2:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_ENTERGS_REQ stReq;
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT1, stReq.aszDeviceID, INPUT_DEFAULT_SIZE);
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT2, stReq.aszNotifyID, INPUT_DEFAULT_SIZE);
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT3, stReq.aszUserName, INPUT_DEFAULT_SIZE);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT4, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwPlayerID = SDAtou(szTemp);
			::GetDlgItemTextA(this->m_hWnd, IDC_EDIT5, szTemp, INPUT_DEFAULT_SIZE);
			stReq.dwCliVer = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT6, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wCareerID = SDAtou(szTemp);
            // 发送报文
            Send_COMMON(CLIGS_ENTERGS_REQ, &stReq, "进入游戏");
        }
        break;
        //获取武将详细信息
        case 4:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_GET_HERO_INFO_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byEquipBagFlag = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwPlayerID = atoi(szTemp);


            // 发送报文
            Send_COMMON(CLIGS_GET_HERO_INFO_REQ, &stReq, "获取武将详细信息");
        }
        break;
        //进入家园
        case 5:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_ENTER_HOME_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byKind = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_ENTER_HOME_REQ, &stReq, "进入家园");
        }
        break;
        //获取场景副本请求
        case 6:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_GET_SCENE_INSTANCE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wIndex = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_GET_SCENE_INSTANCE_REQ, &stReq, "获取场景副本");
        }
        break;
        //进入副本请求
        case 7:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_ENTER_INSTANCE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wSceneIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byTownIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byInstanceIdx = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_ENTER_INSTANCE_REQ, &stReq, "进入副本");
        }
        break;
        //战斗请求
        case 8:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_BATTLE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byBattleIndex = atoi(szTemp);
            // 发送报文
            Send_COMMON(CLIGS_BATTLE_REQ, &stReq, "战斗");

        }
        break;
        //退出副本
        case 9:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_EXIT_INSTANCE_REQ stReq;
            // 发送报文
            Send_COMMON(CLIGS_EXIT_INSTANCE_REQ, &stReq, "退出副本");
        }
        break;
        //建筑升级请求
        case 10:
        {

        }
        break;
        //收获请求
        case 11:
        {

        }
        break;
        //打开铁匠铺请求
        case 12:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_SMITHY_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byBuildKindID = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_SMITHY_REQ, &stReq, "打开铁匠铺");
        }
        break;
        //装备强化请求
        case 13:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_EQUIP_STRENGTHEN_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byStrengthenType = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_EQUIP_STRENGTHEN_REQ, &stReq, "装备强化");
        }
        break;
        //打开招募馆请求
        case 14:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_RECRUIT_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byBuildKindID = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_RECRUIT_REQ, &stReq, "打开铁匠铺");
        }
        break;
        //招募武将请求
        case 15:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_RECRUIT_HERO_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wHeroKindID = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_RECRUIT_HERO_REQ, &stReq, "招募武将");
        }
        break;
        //打开生产建筑请求
        case 16:
        {
        }
        break;
        //打开背包请求
        case 17:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_BAG_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_BAG_REQ, &stReq, "打开背包");
        }
        break;
        //打开科技馆请求
        case 18:

            break;
            //升级科技请求
        case 19:

            break;
            //打开竞技场请求
        case 20:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_RACE_BUILD_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byBuildKindID = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_RACE_BUILD_REQ, &stReq, "打开竞技场");
        }
        break;
        //领取排名请求
        case 21:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_RECV_RANK_ENCOURAGE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_RECV_RANK_ENCOURAGE_REQ, &stReq, "领取排名");
        }
        break;
        //购买道具请求
        case 22:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_BUY_ITEM_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byTabID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wPileCount = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_BUY_ITEM_REQ, &stReq, "购买道具");
        }
        break;
        //清除玩家数据请求
        case 23:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_CLR_PLAYER_DATA_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_CLR_PLAYER_DATA_REQ, &stReq, "清除玩家数据");
        }
        break;
        //领取任务奖励请求
        case 24:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_RECV_TASK_ENCOURAGE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byTaskLineIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwTaskIdx = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_RECV_TASK_ENCOURAGE_REQ, &stReq, "领取任务奖励");
        }
        break;
        //布阵请求
        case 25:
        {
            /*
            CHAR szTemp[128] = {0};
            PKT_CLIGS_EMBATTLE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byEmbattleType = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byFormationIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wUpHeroKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT4, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wDownHeroKindID = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_EMBATTLE_REQ, &stReq, "布阵");
            */
        }
        break;
        //装备穿戴请求
        case 26:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_EQUIP_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wHeroKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byDressType = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wDressItemKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT4, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wDressItemIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT5, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wUndressItemKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT6, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wUndressItemIdx = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_EQUIP_REQ, &stReq, "装备穿戴");
        }
        break;
        //挑战请求
        case 27:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_CHALLENGE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwRank = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_CHALLENGE_REQ, &stReq, "挑战");
        }
        break;
        //获取武将丹药请求
        case 28:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_GET_HERO_DRUG_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wHeroKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byNeedDrugItemFlag = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_GET_HERO_DRUG_REQ, &stReq, "获取武将丹药");
        }
        break;
        //使用丹药请求
        case 29:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_USE_DRUG_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wHeroKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wHeroKindID = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_USE_DRUG_REQ, &stReq, "使用丹药");
        }
        break;
        //打开制作卷请求
        case 30:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_ITEM_MAKER_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wItemKindID = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_ITEM_MAKER_REQ, &stReq, "打开制作卷");
        }
        break;
        //道具制作请求
        case 31:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_MAKE_ITEM_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wItemKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wEquipKindID = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wEquipIdx = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_MAKE_ITEM_REQ, &stReq, "道具制作");
        }
        break;
        //遣散武将请求
        case 32:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_DISBAND_HERO_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wHeroKindID = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_DISBAND_HERO_REQ, &stReq, "遣散武将");
        }
        break;

        //打开副本奖励宝箱请求
        case 33:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_REQ, &stReq, "打开副本奖励宝箱");
        }
        break;
        //挂机请求
        case 34:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_ONHOOK_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.stInstanceID.wSceneIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.stInstanceID.byTownIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.stInstanceID.byInstanceIdx = atoi(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT4, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byRunTimes = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_ONHOOK_REQ, &stReq, "挂机");
        }
        break;
        //领取在线奖励请求
        case 35:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_RECV_ONLINE_ENCOURAGE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = atoi(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_RECV_ONLINE_ENCOURAGE_REQ, &stReq, "领取在线奖励");
        }
        break;
        //注册昵称请求
        case 36:
        {
            CHAR szTemp[128] = {0};
            PKT_CLILS_REGISTER_DSPNAME_REQ stReq;
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT1, stReq.aszDspName, INPUT_DEFAULT_SIZE);

            // 发送报文
            Send_COMMON(CLILS_REGISTER_DSPNAME_REQ, &stReq, "注册昵称");
        }
        break;
        //打开布阵面板请求
        case 37:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_EMBATTLE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = atoi(szTemp);


            // 发送报文
            Send_COMMON(CLIGS_OPEN_EMBATTLE_REQ, &stReq, "打开布阵面板");
        }
        break;
        //物品使用请求
        case 38:
        {
            CHAR szTemp[128] = {0};
            PKT_CLILS_USE_GOODS_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wGoodsKindID = atoi(szTemp);


            // 发送报文
            Send_COMMON(CLILS_USE_GOODS_REQ, &stReq, "物品使用");
        }
        break;
        //时间同步
        case 39:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_SYNC_TIME_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.qwCliTime = SDAtou64(szTemp);


            // 发送报文
            Send_COMMON(CLIGS_SYNC_TIME_REQ, &stReq, "时间同步");
        }
        break;
        //打开登录奖励面板请求
        case 40:
        {
            CHAR szTemp[128] = {0};
            PKT_CLILS_OPEN_LOGIN_ENCOURAGE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);


            // 发送报文
            Send_COMMON(CLILS_OPEN_LOGIN_ENCOURAGE_REQ, &stReq, "打开登录奖励面板");
        }
        break;
        //登录奖励摇奖请求
        case 41:
        {
            CHAR szTemp[128] = {0};
            PKT_CLILS_RATE_LOGIN_ENCOURAGE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);


            // 发送报文
            Send_COMMON(CLILS_RATE_LOGIN_ENCOURAGE_REQ, &stReq, "登录奖励摇奖");
        }
        break;
        //登录奖励放入背包请求
        case 42:
        {
            CHAR szTemp[128] = {0};
            PKT_CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);


            // 发送报文
            Send_COMMON(CLILS_PUT_LOGIN_ENCOURAGE_INTO_BAG_REQ, &stReq, "登录奖励放入背包");
        }
        break;
        //获取竞技场排名请求
        case 43:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_GET_RANK_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);


            // 发送报文
            Send_COMMON(CLIGS_GET_RANK_REQ, &stReq, "获取竞技场排名");
        }
        break;
        //接受任务
        case 44:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_ACCEPT_TASK_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byTaskLineIdx = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwTaskIdx = SDAtou(szTemp);


            // 发送报文
            Send_COMMON(CLIGS_ACCEPT_TASK_REQ, &stReq, "接受任务");
        }
        break;
        //打开俘虏工作面板
        case 45:
        {
        }
        break;
        //指派俘虏工作位置
        case 46:
        {
        }
        break;
        //刷新俘虏工作级别
        case 47:
        {
        }
        break;
        //俘虏工作收获请求
        case 48:
        {
        }
        break;
        //解锁俘虏工作位置
        case 49:
        {
        }
        break;
        //立即清除工作CD
        case 50:
        {
        }
        break;
        //中断工作请求
        case 51:
        {

        }
        break;
        //清除俘虏工作CD请求
        case 52:
        {

        }
        break;
        //释放俘虏请求
        case 53:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_FREE_CAPTURE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwCapturePlayerID = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_FREE_CAPTURE_REQ, &stReq, "释放俘虏");
        }
        break;
        //升级神器请求
        case 54:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_UPGRADE_GODWEAPON_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wHeroKindID = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wQuality = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wLevel = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_UPGRADE_GODWEAPON_REQ, &stReq, "升级神器");
        }
        break;
        //打开掠夺场景请求
        case 55:
        {

        }
        break;
        //掠夺请求
        case 56:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_PLUNDER_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwEnemyID = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwCaptureID = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT3, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byUseItemFlag = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT4, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byResistFlag = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT5, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byCaptureGridIdx = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_PLUNDER_REQ, &stReq, "掠夺");
        }
        break;
        //打开仇敌面板
        case 57:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_ENEMY_TAB_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wStartIdx = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_ENEMY_TAB_REQ, &stReq, "打开仇敌面板");
        }
        break;
        //删除仇敌
        case 58:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_DEL_ENEMY_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.dwEnemyID = SDAtou(szTemp);
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wStartIdx = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_DEL_ENEMY_REQ, &stReq, "删除仇敌");
        }
        break;
        //打开摇钱树
        case 59:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_MONEY_TREE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_MONEY_TREE_REQ, &stReq, "打开摇钱树");
        }
        break;
        //获取掠夺战报
        case 60:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_GET_PLUNDER_BATTLELOG_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_GET_PLUNDER_BATTLELOG_REQ, &stReq, "获取掠夺战报");
        }
        break;
        //打开技能面板
        case 61:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_SKILL_TAB_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_SKILL_TAB_REQ, &stReq, "打开技能面板");
        }
        break;
        //技能升级请求
        case 62:
        {

        }
        break;
        //打开俘虏面板请求
        case 63:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_CAPTURE_TAB_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_CAPTURE_TAB_REQ, &stReq, "打开俘虏面板");
        }
        break;
        //打开丹药服用面板
        case 64:
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_DRUG_USE_TAB_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_DRUG_USE_TAB_REQ, &stReq, "打开丹药服用面板");
        }
        break;
        case 65://打开武将丹药服用面板
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ	stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wKindID = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_HERO_DRUG_USE_TAB_REQ, &stReq, "打开武将丹药服用面板");
        }
        break;
        case 66://清除丹药服用CD
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_CLR_USE_DRUG_CD_REQ	stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wKindID = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_CLR_USE_DRUG_CD_REQ, &stReq, "清除丹药服用CD");
        }
        break;
        case 67://使用丹药
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_USE_DRUG_REQ	stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wHeroKindID = SDAtou(szTemp);

            memset(szTemp, 0x00, sizeof(szTemp));
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT2, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wUseLevel = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_CLR_USE_DRUG_CD_REQ, &stReq, "使用丹药");
        }
        break;
        case 68://打开丹药升级面板
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byExt = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ, &stReq, "打开丹药升级面板");
        }
        break;
        case 69://丹药升级
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGS_COMPOSE_EQUIP_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wEquipItemID = SDAtou(szTemp);

            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wEquipIdx = SDAtou(szTemp);

            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
            stReq.byGoldComposeFlag = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGS_OPEN_UPGRADE_DRUG_TAB_REQ, &stReq, "装备合成");
        }
        break;
        case 70://选区
        {
            CHAR szTemp[128] = {0};
            PKT_CLIGT_SELECT_ZONE_REQ stReq;
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT1, szTemp, INPUT_DEFAULT_SIZE);
			stReq.byAuthType = SDAtou(szTemp);
			::GetDlgItemText(this->m_hWnd, IDC_EDIT2, stReq.aszDeviceID, INPUT_DEFAULT_SIZE);
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT3, stReq.aszUserName, INPUT_DEFAULT_SIZE);			
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT4, (CHAR*)(stReq.abyUserPwd), INPUT_DEFAULT_SIZE);
			stReq.byPwdLen = strlen((CHAR*)(stReq.abyUserPwd));
            ::GetDlgItemTextA(this->m_hWnd, IDC_EDIT5, szTemp, INPUT_DEFAULT_SIZE);
            stReq.wZoneIdx = SDAtou(szTemp);

            // 发送报文
            Send_COMMON(CLIGT_SELECT_ZONE_REQ, &stReq, "选区");
        }
        break;

		case 71://打开悟道面板
			{
				CHAR szTemp[128] = {0};
				PKT_CLIGS_OPEN_AWAKEN_TAB_REQ stReq;

				// 发送报文
				Send_COMMON(CLIGS_OPEN_AWAKEN_TAB_REQ, &stReq, "打开悟道面板");
			}
			break;

        default:
            break;
        }
    }
    else
    {
        PrintText("[ 没有选择协议。]", LOG_ERROR);
    }
}





VOID CPacketRobotDlg::OnRecvMsg(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen)
{
    CHAR szText[LOG_MAX_LENGTH];
    CHAR acTemp[LOG_MAX_LENGTH];
    INT32 nTrack = 0; // 哨兵
    if(0 == nLen)
    {
        return;
    }

    sprintf(szText, "[ 收到数据，MsgID = %d, Len = %d。]", wMsgID, nLen);
    PrintText(szText, LOG_DESCRIBE);
    //sprintf(szText, "[sizeof(PKT_CLIGS_CHALLENGE_ACK) = %d]", sizeof(PKT_CLIGS_CHALLENGE_ACK));
	//sprintf(szText, "[sizeof(DT_JINGJIE_DATA)*4 = %d]", sizeof(DT_JINGJIE_DATA)*4);
    PrintText(szText, LOG_DESCRIBE);
    CSDProtocol *pProtocol = CProtocolMgr::Instance()->FindProtocol(wMsgID);
    if(pProtocol)
    {
        static CHAR DecodeData[PACKAGE_LENGTH] = {0};
        CHAR* cpMsgData = (CHAR*)(pBuf);
        INT32 nRet = pProtocol->Decode(wMsgID, cpMsgData, nLen, DecodeData,	PACKAGE_LENGTH);
        if(nRet <= 0 )
        {
            sprintf(szText, "[ MsgID = %d, Len = %d] decode error!", wMsgID);
            PrintText(szText, LOG_DESCRIBE);
            return;
        }
        sprintf(szText, "[ wMsgID = %d]", wMsgID);
        PrintText(szText, LOG_DESCRIBE);
        switch(wMsgID)
        {
        case CLIGT_ENDEC_KEY_NTF:
        {
            PKT_CLIGT_ENDEC_KEY_NTF* pNtf = (PKT_CLIGT_ENDEC_KEY_NTF*)DecodeData;
            PrintText("PKT_CLIGT_ENDEC_KEY_NTF\r\n{");
            sprintf(acTemp, "\t abyKeyInfo = \"%s\"", pNtf->abyKeyInfo);
            PrintText(acTemp);
            PrintText("}");
            CGlobe::m_poSeesion->SetKey(pNtf->abyKeyInfo, ENDEC_KEY_LEN);
            break;
        }
        case CLIZS_ENDEC_KEY_NTF:
        {
            PKT_CLIZS_ENDEC_KEY_NTF* pNtf = (PKT_CLIZS_ENDEC_KEY_NTF*)DecodeData;
            PrintText("PKT_CLIZS_ENDEC_KEY_NTF\r\n{");
            sprintf(acTemp, "\t abyKeyInfo = \"%s\"", pNtf->abyKeyInfo);
            PrintText(acTemp);
            PrintText("}");
            CGlobe::m_poSeesion->SetKey(pNtf->abyKeyInfo, ENDEC_KEY_LEN);
            break;
        }

        case CLIGT_HEARTBEAT_ACK:
        {
            PKT_CLIGT_HEARTBEAT_ACK* pAck = (PKT_CLIGT_HEARTBEAT_ACK*)DecodeData;
            PrintText("PKT_CLIGT_HEARTBEAT_ACK\r\n{");
            PrintText("}");
        }
        break;
        case CLILS_LOGIN_ACK:
        {
            PKT_CLILS_LOGIN_ACK* pAck = (PKT_CLILS_LOGIN_ACK*)DecodeData;
            PrintText("PKT_CLILS_LOGIN_ACK\r\n{");

            sprintf(acTemp, "\t nErrCode = %d", pAck->wErrCode);
            PrintText(acTemp);
            sprintf(acTemp, "\t aszDeviceID = \"%s\"", _SDTT2Local(pAck->aszDeviceID));
            PrintText(acTemp);
            sprintf(acTemp, "\t aszNotifyID = \"%s\"", _SDTT2Local(pAck->aszNotifyID));
            PrintText(acTemp);
            sprintf(acTemp, "\t aszUserName = \"%s\"", _SDTT2Local(pAck->aszUserName));
            PrintText(acTemp);
            sprintf(acTemp, "\t dwPlayerID = \"%u\"", pAck->dwPlayerID);
            PrintText(acTemp);

            PrintText("}");
            break;
        }


        case CLIGS_ENTERGS_ACK:
        {
            PKT_CLIGS_ENTERGS_ACK* pAck = (PKT_CLIGS_ENTERGS_ACK*)DecodeData;
            PrintText("PKT_CLIGS_ENTERGS_ACK\r\n{");

            sprintf(acTemp, "\t sizeof(PKT_CLIGS_ENTERGS_ACK) = %d", sizeof(PKT_CLIGS_ENTERGS_ACK));
            PrintText(acTemp);

            sprintf(acTemp, "\t nErrCode = %d", pAck->wErrCode);
            PrintText(acTemp);
            if(0 != pAck->wErrCode)
            {
                PrintText("}");
                break;
            }
            sprintf(acTemp, "\t dwID = %d", pAck->stPlayerInfo.dwID);
            PrintText(acTemp);
            sprintf(acTemp, "\t aszDispName = \"%s\"", _SDTT2Local(pAck->stPlayerInfo.aszDispName));
            PrintText(acTemp);
            sprintf(acTemp, "\t qwCoin = %d", pAck->stPlayerInfo.qwCoin);
            PrintText(acTemp);
            sprintf(acTemp, "\t qwGold = %d", pAck->stPlayerInfo.qwGold);
            PrintText(acTemp);
            sprintf(acTemp, "\t wLevel = %d", pAck->stPlayerInfo.wLevel);
            PrintText(acTemp);
            sprintf(acTemp, "\t wPhyStrength = %d", pAck->stPlayerInfo.stPhyStrengthInfo.wPhyStrength);
            PrintText(acTemp);
            sprintf(acTemp, "\t wMaxPhyStrength = %d", pAck->stPlayerInfo.stPhyStrengthInfo.wMaxPhyStrength);
            PrintText(acTemp);
            sprintf(acTemp, "\t wPhyStrengthCountDown = %d", pAck->stPlayerInfo.stPhyStrengthInfo.wPhyStrengthCountDown);
            PrintText(acTemp);
            sprintf(acTemp, "\t wPhyStrengthRefreshCD = %d", pAck->stPlayerInfo.stPhyStrengthInfo.wPhyStrengthRefreshCD);
            PrintText(acTemp);
            sprintf(acTemp, "\t dwExperience = %d", pAck->stPlayerInfo.stExperienceInfo.dwExperience);
            PrintText(acTemp);
            sprintf(acTemp, "\t dwUpgrateExperience = %d", pAck->stPlayerInfo.stExperienceInfo.dwUpgradeExperience);
            PrintText(acTemp);
            sprintf(acTemp, "\t dwScience = %d", pAck->stPlayerInfo.qwScience);
            PrintText(acTemp);

            PrintText("}");
            break;
        }

        case CLIGS_GET_HERO_INFO_ACK:
        {
            PKT_CLIGS_GET_HERO_INFO_ACK* pAck = (PKT_CLIGS_GET_HERO_INFO_ACK*)DecodeData;
            PrintText("PKT_CLIGS_GET_HERO_INFO_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_ENTER_HOME_ACK:
        {
            PKT_CLIGS_ENTER_HOME_ACK* pAck = (PKT_CLIGS_ENTER_HOME_ACK*)DecodeData;
            PrintText("PKT_CLIGS_ENTER_HOME_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_ENTER_INSTANCE_ACK:
        {
            PKT_CLIGS_ENTER_INSTANCE_ACK* pAck = (PKT_CLIGS_ENTER_INSTANCE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_ENTER_INSTANCE_ACK\r\n{");
            sprintf(acTemp, "\t nErrCode = %d", pAck->wErrCode);
            PrintText(acTemp);
            PrintText("}");
            break;
        }
        case CLIGS_BATTLE_ACK:
        {
            PKT_CLIGS_BATTLE_ACK* pAck = (PKT_CLIGS_BATTLE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_BATTLE_ACK\r\n{");
            /*
            sprintf(acTemp, "Myself CurHP:%d\n", pAck->stBattleInfo.stMyselfInfo.stCurTotalBattleAttr.dwHP);
            PrintText(acTemp);
            sprintf(acTemp, "Enemy CurHP:%d\n", pAck->stBattleInfo.stEnemyInfo.stCurTotalBattleAttr.dwHP);
            PrintText(acTemp);
            for(UINT8 byIdx = 0; byIdx < pAck->stBattleInfo.byBattleRoundNum; byIdx++)
            {
                DT_BATTLE_ROUND_DATA& stDT_BATTLE_ROUND_DATA = pAck->stBattleInfo.astBattleRoundInfo[byIdx];

                if(0 == stDT_BATTLE_ROUND_DATA.byAttackerFlag)
                {
                    sprintf(acTemp, "Myself Attack:");
                    PrintText(acTemp);
                }
                else
                {
                    sprintf(acTemp, "Enymy Attack:");
                    PrintText(acTemp);
                }

                sprintf(acTemp, "\t stBeforeAttack.dwMyselfBuffState:[%d][%d]", stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack.astMyselfBuffState[0].wSkillID, stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack.astMyselfBuffState[0].byBuffID);
                PrintText(acTemp);
                sprintf(acTemp, "\t stAfterAttack.dwMyselfBuffState:[%d][%d]\n", stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack.astMyselfBuffState[0].wSkillID, stDT_BATTLE_ROUND_DATA.stAttackData.stAfterAttack.astMyselfBuffState[0].byBuffID);
                PrintText(acTemp);
                sprintf(acTemp, "\t stBeforeAttack.byCritDodge:%d", stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack.byCritDodge);
                PrintText(acTemp);
                sprintf(acTemp, "\t stBeforeAttack.byMyselfHpChangeNum:%d, anMyselfHpChangeInfo[0]:%d",
                	stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack.byMyselfHpChangeNum, stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack.anMyselfHpChangeInfo[0]);
                PrintText(acTemp);
                sprintf(acTemp, "\t stBeforeAttack.byEnemyHpChangeNum:%d, anEnemyHpChangeInfo[0]:%d\n",
                	stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack.byEnemyHpChangeNum, stDT_BATTLE_ROUND_DATA.stAttackData.stBeforeAttack.anEnemyHpChangeInfo[0]);
                PrintText(acTemp);

                sprintf(acTemp, "\t stAfterAttack.byCritDodge:%d", stDT_BATTLE_ROUND_DATA.stAttackData.stAfterAttack.byCritDodge);
                PrintText(acTemp);
                sprintf(acTemp, "\t stAfterAttack.byMyselfHpChangeNum:%d, anMyselfHpChangeInfo[0]:%d",
                	stDT_BATTLE_ROUND_DATA.stAttackData.stAfterAttack.byMyselfHpChangeNum, stDT_BATTLE_ROUND_DATA.stAttackData.stAfterAttack.anMyselfHpChangeInfo[0]);
                PrintText(acTemp);
                sprintf(acTemp, "\t stAfterAttack.byEnemyHpChangeNum:%d, anEnemyHpChangeInfo[0]:%d\n",
                	stDT_BATTLE_ROUND_DATA.stAttackData.stAfterAttack.byEnemyHpChangeNum, stDT_BATTLE_ROUND_DATA.stAttackData.stAfterAttack.anEnemyHpChangeInfo[0]);
                PrintText(acTemp);


            }
            */
            PrintText("}");
            break;
        }
        case CLIGS_EXIT_INSTANCE_ACK:
        {
            PKT_CLIGS_EXIT_INSTANCE_ACK* pAck = (PKT_CLIGS_EXIT_INSTANCE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_EXIT_INSTANCE_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_GET_SCENE_INSTANCE_ACK:
        {
            PKT_CLIGS_GET_SCENE_INSTANCE_ACK* pAck = (PKT_CLIGS_GET_SCENE_INSTANCE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_GET_SCENE_INSTANCE_ACK\r\n{");

            PrintText("}");
            break;
        }

        case CLIGS_OPEN_SMITHY_ACK:
        {
            PKT_CLIGS_OPEN_SMITHY_ACK* pAck = (PKT_CLIGS_OPEN_SMITHY_ACK*)DecodeData;
            PrintText("PKT_CLIGS_OPEN_SMITHY_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_EQUIP_STRENGTHEN_ACK:
        {
            PKT_CLIGS_EQUIP_STRENGTHEN_ACK* pAck = (PKT_CLIGS_EQUIP_STRENGTHEN_ACK*)DecodeData;
            PrintText("PKT_CLIGS_EQUIP_STRENGTHEN_ACK\r\n{");

            PrintText("}");
            break;
        }

        case CLIGS_OPEN_RECRUIT_ACK:
        {
            PKT_CLIGS_OPEN_RECRUIT_ACK* pAck = (PKT_CLIGS_OPEN_RECRUIT_ACK*)DecodeData;
            PrintText("PKT_CLIGS_OPEN_RECRUIT_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_RECRUIT_HERO_ACK:
        {
            PKT_CLIGS_RECRUIT_HERO_ACK* pAck = (PKT_CLIGS_RECRUIT_HERO_ACK*)DecodeData;
            PrintText("PKT_CLIGS_RECRUIT_HERO_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_OPEN_BAG_ACK:
        {
            PKT_CLIGS_OPEN_BAG_ACK* pAck = (PKT_CLIGS_OPEN_BAG_ACK*)DecodeData;
            PrintText("PKT_CLIGS_OPEN_BAG_ACK\r\n{");

            PrintText("}");
            break;
        }

        case CLIGS_OPEN_RACE_BUILD_ACK:
        {
            PKT_CLIGS_OPEN_RACE_BUILD_ACK* pAck = (PKT_CLIGS_OPEN_RACE_BUILD_ACK*)DecodeData;
            PrintText("PKT_CLIGS_OPEN_RACE_BUILD_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_RECV_RANK_ENCOURAGE_ACK:
        {
            PKT_CLIGS_RECV_RANK_ENCOURAGE_ACK* pAck = (PKT_CLIGS_RECV_RANK_ENCOURAGE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_RECV_RANK_ENCOURAGE_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_BUY_ITEM_ACK:
        {
            PKT_CLIGS_BUY_ITEM_ACK* pAck = (PKT_CLIGS_BUY_ITEM_ACK*)DecodeData;
            PrintText("PKT_CLIGS_BUY_ITEM_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_RECV_TASK_ENCOURAGE_ACK:
        {
            PKT_CLIGS_RECV_TASK_ENCOURAGE_ACK* pAck = (PKT_CLIGS_RECV_TASK_ENCOURAGE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_RECV_TASK_ENCOURAGE_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_EMBATTLE_ACK:
        {
            PKT_CLIGS_EMBATTLE_ACK* pAck = (PKT_CLIGS_EMBATTLE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_EMBATTLE_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_EQUIP_ACK:
        {
            PKT_CLIGS_EQUIP_ACK* pAck = (PKT_CLIGS_EQUIP_ACK*)DecodeData;
            PrintText("PKT_CLIGS_EQUIP_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_CHALLENGE_ACK:
        {
            PKT_CLIGS_CHALLENGE_ACK* pAck = (PKT_CLIGS_CHALLENGE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_CHALLENGE_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_GET_HERO_DRUG_ACK:
        {
            PKT_CLIGS_GET_HERO_DRUG_ACK* pAck = (PKT_CLIGS_GET_HERO_DRUG_ACK*)DecodeData;
            PrintText("PKT_CLIGS_GET_HERO_DRUG_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_USE_DRUG_ACK:
        {
            PKT_CLIGS_USE_DRUG_ACK* pAck = (PKT_CLIGS_USE_DRUG_ACK*)DecodeData;
            PrintText("PKT_CLIGS_USE_DRUG_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_OPEN_ITEM_MAKER_ACK:
        {
            PKT_CLIGS_OPEN_ITEM_MAKER_ACK* pAck = (PKT_CLIGS_OPEN_ITEM_MAKER_ACK*)DecodeData;
            PrintText("PKT_CLIGS_OPEN_ITEM_MAKER_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_MAKE_ITEM_ACK:
        {
            PKT_CLIGS_MAKE_ITEM_ACK* pAck = (PKT_CLIGS_MAKE_ITEM_ACK*)DecodeData;
            PrintText("PKT_CLIGS_MAKE_ITEM_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_DISBAND_HERO_ACK:
        {
            PKT_CLIGS_DISBAND_HERO_ACK* pAck = (PKT_CLIGS_DISBAND_HERO_ACK*)DecodeData;
            PrintText("PKT_CLIGS_DISBAND_HERO_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK:
        {
            PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK* pAck = (PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK*)DecodeData;
            PrintText("PKT_CLIGS_OPEN_INSTANCE_ENCOURAGE_BOX_ACK\r\n{");

            PrintText("}");
            break;
        }
        case CLIGS_OPEN_EMBATTLE_ACK:
        {
            PKT_CLIGS_OPEN_EMBATTLE_ACK* pAck = (PKT_CLIGS_OPEN_EMBATTLE_ACK*)DecodeData;
            PrintText("PKT_CLIGS_OPEN_EMBATTLE_ACK\r\n{");

            PrintText("}");
            break;

        }
        case CLIGS_SYNC_TIME_ACK:
        {
            PKT_CLIGS_SYNC_TIME_ACK* pAck = (PKT_CLIGS_SYNC_TIME_ACK*)DecodeData;
            PrintText("PKT_CLIGS_SYNC_TIME_ACK\r\n{");

            PrintText("}");
            break;

        }

        case CLIGT_SELECT_ZONE_ACK:
        {
            PKT_CLIGT_SELECT_ZONE_ACK* pAck = (PKT_CLIGT_SELECT_ZONE_ACK*)DecodeData;
            PrintText("PKT_CLIGT_SELECT_ZONE_ACK\r\n{");

            PrintText("}");
            break;
        }

        case CLILS_BIND_ACK:
        {
            PKT_CLILS_BIND_ACK* pAck = (PKT_CLILS_BIND_ACK*)DecodeData;
            PrintText("PKT_CLILS_BIND_ACK\r\n{");
            PrintText("}");
            break;
        }
        break;
		case CLIGS_OPEN_AWAKEN_TAB_ACK:
		{
			PKT_CLIGS_OPEN_AWAKEN_TAB_ACK* pAck = (PKT_CLIGS_OPEN_AWAKEN_TAB_ACK*)DecodeData;
			PrintText("PKT_CLIGS_OPEN_AWAKEN_TAB_ACK\r\n{");
			PrintText("}");
			break;
		}

        default:
        {
            PrintText("[ 未处理协议。]", LOG_ERROR);
        }
        }
    }
    else
    {
        PrintText("[ 协议查找失败。]", LOG_ERROR);
    }
}
