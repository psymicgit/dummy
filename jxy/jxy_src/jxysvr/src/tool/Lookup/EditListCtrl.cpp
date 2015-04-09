// EditListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Lookup.h"
#include "EditListCtrl.h"


// CEditListCtrl

#define ID_LISTEDIT WM_USER + 301

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)

CEditListCtrl::CEditListCtrl()
{
    m_edit = NULL;
    m_item = -1;
    m_subitem = -1;
}

CEditListCtrl::~CEditListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CLICK, &CEditListCtrl::OnNMClick)
    ON_NOTIFY_REFLECT(NM_SETFOCUS, &CEditListCtrl::OnNMSetfocus)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_NOTIFY_REFLECT(NM_RCLICK, &CEditListCtrl::OnNMRClick)
END_MESSAGE_MAP()



// CEditListCtrl 消息处理程序

void CEditListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(!m_edit)
    {
        m_subitem = pNMListView->iSubItem;
        if(pNMListView->iItem != m_item)
        {
            m_item = pNMListView->iItem; //标志被单击的项目
            return;
        }
    }

    RECT m_itemrect, m_r;
    GetItemRect(m_item , &m_itemrect, 2);
    GetItemRect(0 , &m_r, 2);
    int m_height = m_itemrect.bottom - m_itemrect.top ;
    int x = m_r.left , y = m_r.top, cx, cy; //(x,y,cx,cy)为编辑框显示的位置
    for(int i = 0; i < m_item; i++)
        y += m_height;
    cy = y + m_height;
    int i = 0;
    for(i = 0; i < m_subitem; i++)
        x += GetColumnWidth(i);
    cx = x + GetColumnWidth(i);
    if(m_edit)//若编辑框已存在。
    {
        CString s1;
        s1.Format ("%d %d %d %d", x, y, cx, cy);
        m_edit->MoveWindow(x, y, cx - x, cy - y); //移动到当前子项目的位置。
        Invalidate();//刷新屏幕。
        return;
    }
    //若编辑框不存在,则创建编辑框,并在当前子项目处显示编辑框。
    CRect rect(x, y, cx, cy);
    m_edit = new CEdit();
    m_edit->Create (WS_CHILD | WS_VISIBLE | WS_BORDER, rect, this, ID_LISTEDIT);
    CString str = GetItemText (pNMListView->iItem, pNMListView->iSubItem);
    m_edit->UpdateData(0);
    m_edit->SetWindowText(str);
    DWORD dwSel = m_edit->GetSel();
    m_edit->SetSel(HIWORD(dwSel), -1);
    m_edit->ShowWindow (SW_SHOW);//显示编辑框。
    m_edit->SetFocus ();
    *pResult = 0;
}


void CEditListCtrl::OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    if(m_edit)
    {
        //将编辑框中的数据写回对应的子项目中
        UpdateData( );
        CString str;
        m_edit->GetWindowText(str);
        SetItemText(m_item, m_subitem, str);
        delete m_edit;
        m_edit = NULL;
    }
    *pResult = 0;
}


void CEditListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //水平滚动时,移动已显示的编辑框。
    CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
    LRESULT* pResult = new LRESULT;
    if(m_edit)
        OnNMClick((NMHDR*)this, pResult) ;
}


void CEditListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    //垂直滚动时,移动已显示的编辑框。
    CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
    LRESULT* pResult = new LRESULT;
    if(m_edit)
    {
        RECT m_itemrect, m_headrect;
        GetItemRect(m_item , &m_itemrect, 2);
        GetHeaderCtrl()->GetWindowRect(&m_headrect);
        if(m_itemrect.top < m_headrect.bottom - m_headrect.top)
        {
            RECT m_rect;
            m_edit->GetWindowRect(&m_rect);
            m_edit->MoveWindow(m_rect.left, -(m_rect.bottom - m_rect.top), m_rect.right, 0);
        }
        else
        {
            OnNMClick((NMHDR*)this, pResult) ;
        }
    }
}

BOOL CEditListCtrl::DestroyWindow()
{
    if (m_edit)
    {
        delete m_edit;
    }
    // TODO: 在此添加专用代码和/或调用基类
    return CListCtrl::DestroyWindow();
}

void CEditListCtrl::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    POINT pt = {0};
    GetCursorPos(&pt);//得到鼠标点击位置
    *pResult = 0;
}


void CEditListCtrl::OnAddLine()
{
    // TODO: 在此添加命令处理程序代码
    int nCount = GetItemCount();
    InsertItem(nCount, "(NULL)");
}


void CEditListCtrl::OnDeleteLine()
{
    // TODO: 在此添加命令处理程序代码
    int nSelIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
    if(nSelIndex == -1)
        return;
    DeleteItem(nSelIndex);
}
