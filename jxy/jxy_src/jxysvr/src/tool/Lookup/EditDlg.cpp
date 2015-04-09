// EditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EditDlg.h"
#include "afxdialogex.h"


// CEditDlg 对话框

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
    : CDialogResize(CEditDlg::IDD, pParent)
    , m_strSQL(_T(""))
{

}

CEditDlg::~CEditDlg()
{
    while(!m_LogQueue.IsEmpty())
    {
        CLogMsg *pLogMsg = (CLogMsg *)m_LogQueue.RemoveHead();
        delete pLogMsg;
    }
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogResize::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SQL, m_strSQL);
    DDX_Control(pDX, IDC_LIST_LOG, m_TraceList);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogResize)
    ON_MESSAGE(WM_ADDTRACELINE, OnAddTraceLine)
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CEditDlg)
DLGRESIZE_CONTROL(IDC_EDIT_SQL, DLSZ_SIZE_X )
DLGRESIZE_CONTROL(IDC_EDIT_SQL, DLSZ_SIZE_Y )
DLGRESIZE_CONTROL(IDC_LIST_LOG, DLSZ_SIZE_X)
DLGRESIZE_CONTROL(IDC_LIST_LOG, DLSZ_MOVE_Y)
END_DLGRESIZE_MAP()
// CEditDlg 消息处理程序

BOOL CEditDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    InitResizing(FALSE);
    return TRUE;
}

void CEditDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogResize::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
}

void CEditDlg::AddTraceLine(int nLevel, LPCTSTR pstrFormat, ...)
{
    CString str;

    // format and write the data we were given
    va_list args;
    va_start(args, pstrFormat);
    str.FormatV(pstrFormat, args);

    try
    {
        // create new message
        CLogMsg *pLogMsg = new CLogMsg;

        GetLocalTime(&pLogMsg->m_sysTime);
        pLogMsg->m_nLevel = nLevel;
        pLogMsg->m_strText = str;

        m_QueueLock.Lock();
        m_LogQueue.AddTail(pLogMsg);
        m_QueueLock.Unlock();

        // schedule log action
        PostMessage(WM_ADDTRACELINE);
    }
    catch(...)
    {

    }
}


LRESULT CEditDlg::OnAddTraceLine(WPARAM, LPARAM)
{
    CLogMsg *pLogMsg;

    try
    {
        // get first message in the queue
        m_QueueLock.Lock();
        pLogMsg = (CLogMsg *)m_LogQueue.RemoveHead();
        m_QueueLock.Unlock();

        CString strDateTime;
        strDateTime.Format("%02d/%02d/%02d %02d:%02d:%02d.%03d ",
                           pLogMsg->m_sysTime.wDay, pLogMsg->m_sysTime.wMonth, pLogMsg->m_sysTime.wYear,
                           pLogMsg->m_sysTime.wHour, pLogMsg->m_sysTime.wMinute, pLogMsg->m_sysTime.wSecond,
                           pLogMsg->m_sysTime.wMilliseconds);

        switch(pLogMsg->m_nLevel)
        {
        case 1:
            m_TraceList.AddString(strDateTime + pLogMsg->m_strText, RGB(0, 130, 0));
            break;
        case 2:
            m_TraceList.AddString(strDateTime + pLogMsg->m_strText, RGB(0, 0, 255));
            break;
        case 3:
            m_TraceList.AddString(strDateTime + pLogMsg->m_strText, RGB(255, 0, 0));
            break;
        default:
            m_TraceList.AddString(strDateTime + pLogMsg->m_strText, RGB(0, 0, 0));
            break;

        }
        m_TraceList.SetCurSel(m_TraceList.GetCount() - 1);
        delete pLogMsg;
    }
    catch(...)
    {
        // something is wrong...
    }
    return TRUE;
}

