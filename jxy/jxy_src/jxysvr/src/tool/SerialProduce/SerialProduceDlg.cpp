// SerialProduceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialProduce.h"
#include "SerialProduceDlg.h"
#include "SkinMagicLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialProduceDlg dialog

CSerialProduceDlg::CSerialProduceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialProduceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialProduceDlg)
	m_strActivityID = _T("");
	m_strStart = _T("100");
	m_strEnd = _T("200");
	m_strShow = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCreateState = FALSE;
	
}

void CSerialProduceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialProduceDlg)
	DDX_Control(pDX, IDC_EDIT_SHOW, m_ctrShow);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strActivityID);
	DDX_Text(pDX, IDC_EDIT_START, m_strStart);
	DDX_Text(pDX, IDC_EDIT_END, m_strEnd);
	DDX_Text(pDX, IDC_EDIT_SHOW, m_strShow);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialProduceDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialProduceDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_B_CREAT, OnBCreat)
	ON_BN_CLICKED(IDC_B_OUTPUT, OnBOutput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialProduceDlg message handlers

BOOL CSerialProduceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	

	//打开初始化文件
	UpdateData(TRUE);
    CString strIniTem;
	strIniTem += "SerialProduce.ini";
	TCHAR szExePath[MAX_PATH];
	ZeroMemory(szExePath, MAX_PATH);
	GetModuleFileName(NULL, szExePath, MAX_PATH);
	
	m_strExePath = szExePath;
    m_strExePath = m_strExePath.Left(m_strExePath.ReverseFind(_T('\\')));
    m_strIniPath = m_strExePath + _T('\\') + strIniTem;
	
	if(m_File.Open(m_strIniPath,CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate))
	{	
		
		CString strResult;
		int nLen = 0;
		CString strTitleTem,strStartTemp,strEndTemp;
		int pos1 = 0,pos2 = 0;
		
		m_File.ReadString(strResult);
		nLen = strResult.GetLength();	
		pos1 = strResult.Find(',',0);
		strTitleTem = strResult.Left(pos1);	
		pos2 = strResult.Find(',',pos1 + 1);
		
		//读取保存到文件的序列号标题,并且更新界面
		m_strActivityID = strTitleTem;	
			
		//读取保存到文件的序列号开始和结束数值,并且更新界面
		m_strStart = strResult.Mid(pos1 + 1 ,pos2 - pos1 - 1 );
		m_strEnd = strResult.Right(nLen - pos2 - 1);
		UpdateData(FALSE);  
		m_File.Close();
	}
	else
	{
		AfxMessageBox("打开文件失败!");
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialProduceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialProduceDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialProduceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSerialProduceDlg::OnBCreat() 
{
	// TODO: Add your control notification handler code here

    UpdateData(TRUE);
	if(m_strActivityID.IsEmpty() || m_strStart.IsEmpty() || m_strEnd.IsEmpty())
	{
       AfxMessageBox("请确认是否已经作好必要的设置!");
	   return;
	}
	//显示要生成的序列号的个数
    unsigned long nStart = 0,nEnd = 0,nCount = 0;
	sscanf(m_strStart,"%d",&nStart);
	sscanf(m_strEnd,"%d",&nEnd);
	if(nEnd < nStart)
	{
		AfxMessageBox("数值范围不对!");
		return;
	}
    nCount = nEnd - nStart + 1;
    if(nCount > 100000)
	{
		AfxMessageBox("范围过大!");
		return;
	}
	SetDlgItemInt(IDC_STATIC_COUNT,nCount);
	
	//把计算出来的序列号显示到控件上	
	char  szDecBuffer[200];

    ZeroMemory(szDecBuffer,200);
	m_strShow.Empty();
	m_strSaveToFile.Empty();
	m_strSaveToFile.Empty();

	int nNum = 0;
	for(unsigned long i = nStart; i <= nEnd; i ++)
	{  
		sprintf(szDecBuffer,"%03d",i);

		srand((UINT32)(time(NULL) + rand())); //初始化随机数的种子
		DWORD dwRand = rand();
		char szRand[56] = {0};
		sprintf(szRand, "%05u", dwRand);


		CString strKey = (CString)szDecBuffer  + m_strActivityID + (CString)szRand;
		strKey.MakeUpper();
		if(nNum % 100 == 0 )
		{
			if(m_strSaveToFile.GetLength())
				m_strSaveToFile += ";\r";
			m_strSaveToFile += "insert into activityencouragecdkey(ActivityID, CdKey, UserFlag) values";
		}
		else
		{
			m_strSaveToFile += ",";
		}

		m_strSaveToFile += "(" + m_strActivityID + ",'" + strKey  + "',0)"; 
		
		m_strShow += strKey + "\r\n";
		nNum++;
	}   
	UpdateData(FALSE);	

	//保存配置文件
    CString strIniTem;
	DeleteNotNumber(m_strStart);
	DeleteNotNumber(m_strEnd);
	strIniTem = m_strActivityID + ',' + m_strStart + ',' + m_strEnd + "\r\n";
	if(m_File.Open(m_strIniPath,CFile::modeReadWrite|CFile::modeCreate))
	{
		m_File.Write(strIniTem,strIniTem.GetLength());
		m_File.Close();
	}
	else
	{
		AfxMessageBox("保存配置数据不成功!");
	}
	m_bCreateState = TRUE;
	
}

void CSerialProduceDlg::OnBOutput() 
{
	// TODO: Add your control notification handler code here
    
	//输出到文件
	if(TRUE == m_bCreateState)
	{
		UpdateData(TRUE);
		CString strOutPutPath,strTem;
		DeleteNotNumber(m_strStart);
		DeleteNotNumber(m_strEnd);
		strTem += "sn_" + m_strActivityID + "_" +  m_strStart + "_" +m_strEnd + ".csv";
		strOutPutPath = m_strExePath + _T('\\') + strTem;
		if(m_File.Open(strOutPutPath,CFile::modeReadWrite|CFile::modeCreate))
		{		
			m_File.WriteString(m_strSaveToFile);
			m_File.Close();		
			UpdateData(FALSE);
			AfxMessageBox("数据输出成功!");
		}
		else
		{
			AfxMessageBox("打开文件失败!");
		}
	}
	else
	{
		AfxMessageBox("还没有生成序列号!");
	}
}


