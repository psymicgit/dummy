#include <Windows.h>


int main()
{
    HWND wnd_handle = FindWindow(NULL, "∑Ω”Ó");
    if(NULL == wnd_handle)
    {
        return -1;
    }

    HWND chat_log_dlg = GetDlgItem(wnd_handle, 3);
    if(NULL == chat_log_dlg)
    {
        return -1;
    }

    char buf[256];
    GetWindowText(chat_log_dlg, buf, sizeof(buf));

    return 0;
}