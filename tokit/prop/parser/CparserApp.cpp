
// parser.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CparserApp.h"
#include "parserDlg.h"

#include "parser.h"
#include "echoutil.h"
#include "gen_xsd.h"
#include "gen_h.h"
#include "gen_cpp.h"
#include "check_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CparserApp

BEGIN_MESSAGE_MAP(CparserApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CparserApp construction

CparserApp::CparserApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CparserApp object

CparserApp theApp;

void CparserApp::RunOnConsole()
{
	std::string cmd = __argv[1];
	if(cmd == "-xsd")
	{
		if(__argc != 4)
		{
			ECHO_ERR("生成xsd的命令错误，格式应为: ");
			ECHO_ERR("   -xsd excel结构定义文件的路径 生成的xsd文件放到哪个文件夹下");
			ECHO_ERR("   比如: -xsd   ../../xxxx.xlsx   /yyyy/");

			return;
		}

		std::string xlsx	 = __argv[2];
		std::string xsd_dir  = __argv[3];
		genxsdutil::gen_xsds(xlsx, xsd_dir);
	}
	else if(cmd == "-c++.h")
	{
		if(__argc != 5)
		{
			ECHO_ERR("生成c++头文件的命令错误，格式应为: ");
			ECHO_ERR("   -c++.h excel结构定义文件的路径 h模板文件 生成的.h文件放在哪个文件夹下");
			ECHO_ERR("   比如: -c++.h   ../../xxxx.xlsx  ../h_template.h   ./c++/h/");

			return;
		}

		std::string xlsx	  = __argv[2];
		std::string h_templet = __argv[3];
		std::string h_dir	  = __argv[4];
		cpputil::gen_hs(xlsx, h_dir, h_templet);
	}
	else if(cmd == "-c++.cpp ")
	{
		if(__argc != 5)
		{
			ECHO_ERR("生成c++源代码文件命令错误，格式应为: ");
			ECHO_ERR("   -c++.cpp excel结构定义文件的路径 cpp模板文件 生成的cpp文件放在哪个文件夹下");
			ECHO_ERR("   比如: -c++.cpp  ../../xxxx.xlsx  ../cpp_template.cpp   ./c++/cpp/");

			return;
		}

		std::string xlsx	    = __argv[2];
		std::string cpp_templet = __argv[3];
		std::string cpp_dir	    = __argv[4];
		cpputil::gen_cpps(xlsx, cpp_dir, cpp_templet);
	}
	else if(cmd == "-go")
	{
		if(__argc != 5)
		{
			ECHO_ERR("生成go源代码文件命令错误，格式应为: ");
			ECHO_ERR("   -go excel结构定义文件的路径 go模板文件 生成的go文件放在哪个文件夹下");
			ECHO_ERR("   比如: -go  ../../xxxx.xlsx  ../go_template.go   ./go/");

			return;
		}

		std::string xlsx	    = __argv[2];
		std::string go_templet = __argv[3];
		std::string go_dir	    = __argv[4];
	}
	else if(cmd == "-check")
	{
		if(__argc != 4)
		{
			ECHO_WARN("命令错误，格式应为: ");
			ECHO_WARN("   -check xlsx结构定义文件 xml数据文件的路径");
			ECHO_WARN("   比如: -check ../item.xlsx ../item.xml");
			
			return;
		}

		std::string xlsx	 = __argv[2];
		std::string data_dir = __argv[3];

		checkutil::check(xlsx, data_dir);
	}
}

// CparserApp initialization

BOOL CparserApp::InitInstance()
{
	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    if(__argc >= 2)
    {
        RunOnConsole();
        return FALSE;
    }

	CparserDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}