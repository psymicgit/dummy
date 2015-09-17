//#include <WinDef.h>

#include <Windows.h>
// #include <afx.h>
#include <iostream>
#include <atlcomcli.h>
#include <Mshtmlc.h>
#include <oleacc.h>
#include <comdef.h>
#include <string>
#include <locale>

#include "str_util.h"

BOOL CALLBACK lpEnumProc(HWND hwnd /*窗口内控件句柄*/, LPARAM lParam)
{
    if (NULL == hwnd)
    {
        return FALSE;
    }

    wchar_t buf[501]={0};
    LRESULT nRecvNum=0;

    if (::IsWindow(hwnd)/* && ::IsWindowVisible(hwnd) */)
    {
        EnumChildWindows(hwnd, (WNDENUMPROC)lpEnumProc, NULL);

        GetClassName(hwnd, buf, 500); 
        if(::GetWindowTextLength(hwnd)==0)        //如果控件是editBox，则需要通过发送消息获得
        {
            nRecvNum=::SendMessage(hwnd, WM_GETTEXT, (WPARAM)500, (LPARAM)buf);
            if(nRecvNum==0)
            {
                return TRUE;
            }
            else
            {
                std::wcout << buf << std::endl;
                return TRUE;
            }
        }
        else
        {
            ::GetClassName(hwnd, buf, 500);         //获得控件上的内容
            std::wcout << buf << std::endl;

        }
    }
    else
    {
        ::GetClassName(hwnd, buf, 500);         //获得控件上的内容
        std::wcout << buf << std::endl;

    }

    //TRACE(buffer);
     return TRUE;
}

void IterateByEnum(wchar_t *className, wchar_t *captical)
{
    HWND wnd_handle = FindWindow(className, captical);
    if(NULL == wnd_handle)
    {
        return;
    }

    wchar_t buf[256];
    GetClassName(wnd_handle, buf, 500); 
    std::wcout << buf << std::endl;

    //GetWindowText(wnd_handle, buf, sizeof(buf));

    EnumChildWindows(wnd_handle, (WNDENUMPROC)lpEnumProc, NULL);
}

int GetDepth(HWND hChild)
{
    int depth = 0;
    HWND hParent = GetParent(hChild);
    while(hParent)        
    {
        hParent = GetParent(hParent);
        depth++;
    }

    return depth;
}

void EnumChild(HWND hParent)
{
    HWND hChild = GetWindow(hParent, GW_CHILD);
    wchar_t buf[256];
    wchar_t wnd_text[256] = {0};

    while (hChild)
    {
        if (GetClassName(hChild, buf, sizeof(buf)))
        {
            int depth = GetDepth(hChild);

            for(int idx = 0; idx < depth; idx++)
            {
                std::cout << "  ";
            }
            
            std::wcout << buf /*<< L":" << (wcslen(wnd_text) ? wnd_text : L"") */<< std::endl;
            EnumChild(hChild);
        }

        hChild = GetWindow(hChild, GW_HWNDNEXT);
    }
}

void IterateByWalk(wchar_t *className, wchar_t *captical)
{
    HWND hParent = FindWindow(className, captical);
    EnumChild(hParent);
}

HWND FindChild(HWND hParent, wchar_t *find_name)
{
    if(NULL == hParent)
    {
        return NULL;
    }

    HWND hFind = FindWindowEx(hParent, NULL, find_name, NULL);
    if(NULL == hFind)
    {// not found
        HWND hChild = GetWindow(hParent, GW_CHILD);
        while (hChild)
        {
            hFind = FindChild(hChild, find_name);
            if(hFind)
            {
                return hFind;
            }

            hChild = GetWindow(hChild, GW_HWNDNEXT);
        }

        return NULL;
    }

    return hFind;
}

HWND FindHwndByName(wchar_t *root, wchar_t *find_child)
{
    HWND hParent = FindWindow(root, NULL);
    HWND hFind = FindChild(hParent, find_child);
    return hFind;
}

std::string fetch_chat_history(HWND hwnd_chat_history)
{
    std::string str;  

    CComPtr<IHTMLDocument2> pDoc2;

    HINSTANCE hinst=::LoadLibrary(L"OLEACC.DLL"); 
    if(hinst!=NULL) {  
        LRESULT lres;  
        UINT unMsg=::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));  
        ::SendMessageTimeout(hwnd_chat_history,unMsg,0L,0L,SMTO_ABORTIFHUNG,1000,(DWORD*)&lres);  

        LPFNOBJECTFROMLRESULT pfObjectFromLresult=(LPFNOBJECTFROMLRESULT)::GetProcAddress(hinst, "ObjectFromLresult");  
        if(pfObjectFromLresult!=NULL) {  
            HRESULT hres;  
            hres=(*pfObjectFromLresult)(lres,IID_IHTMLDocument2,0,(void**)&pDoc2);  

            if(SUCCEEDED(hres)) {  

                CComPtr<IHTMLElement> pHtmlElem;  
                hres=pDoc2->get_body(&pHtmlElem);  

                BSTR bstrText=NULL;  
                pHtmlElem->get_innerText(&bstrText);  
                _bstr_t _bstrTemp(bstrText, true);  

                str = _bstrTemp;
            }  
        }  
        ::FreeLibrary(hinst);  
    }  

    return str;
}

int main()
{
    //IterateByEnum(L"TXGFLayerMask", NULL);

    //IterateByWalk(L"StandardWindow", NULL);

    std::locale loc( "chs" );
    std::wcout.imbue( loc ); // 设置为中文环境，让控制台显示出中文

    

    wchar_t root_class_name[] = L"StandardFrame";
    wchar_t find_class_name[] = L"Internet Explorer_Server";
    HWND hwnd_chat_log = FindHwndByName(root_class_name, find_class_name);
    if(NULL == hwnd_chat_log)
    {
        std::wcout << "找不到" << find_class_name << std::endl;
        return 0;
    }

    HWND hRoot = FindWindow(root_class_name, NULL);

    wchar_t buf[512]={0};
    GetWindowText(hRoot, buf, sizeof(buf));

    std::wcout << buf << std::endl;

    CoInitialize(NULL);  
    std::string str = fetch_chat_history(hwnd_chat_log);
    CoUninitialize(); 

    std::cout << str << std::endl;

    system("pause");
    return 0;
}