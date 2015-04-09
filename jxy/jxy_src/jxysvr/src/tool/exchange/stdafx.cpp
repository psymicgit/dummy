
// stdafx.cpp : 只包括标准包含文件的源文件
// Exchange.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

void ConvertUtf8ToGBK(CString& strUtf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

	strUtf8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}


void ConvertGBKToUtf8(CString& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL, 0);
	unsigned short * wszUtf8 = new unsigned short[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL, NULL);

	strGBK = szUtf8;
	delete[] szUtf8;
	delete[] wszUtf8;
}

DWORD GetKey(DWORD _dwkey, DWORD _dwValue)
{
	DWORD dwKey = _dwkey;
	dwKey = dwKey << 16;
	dwKey += _dwValue;
	return dwKey;
}

/********************************************************************/
/*																	*/
/* Function name : AutoSizeColumns									*/
/* Description   : 													*/
/*																	*/
/********************************************************************/
void AutoSizeColumns(CListCtrl *pListCtrl) 
{
	// Call this after your the control is filled
	pListCtrl->SetRedraw(FALSE);
	int mincol = 0;
	int maxcol = pListCtrl->GetHeaderCtrl()->GetItemCount()-1;
	for (int col = mincol; col <= maxcol; col++) 
	{
		pListCtrl->SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = pListCtrl->GetColumnWidth(col);
		pListCtrl->SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = pListCtrl->GetColumnWidth(col);
		// 10 is minumim column width
		int wc = max(10, max(wc1,wc2));
		pListCtrl->SetColumnWidth(col,wc);
	}
	pListCtrl->SetRedraw(TRUE);
}

/*********************************************************************************************
 *函   数   名：十六进制字符串转ASCII
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：
 *修 改  记 录：NONE
 *********************************************************************************************/
int HexToAscii(unsigned char *to, unsigned char *from, unsigned int len) {
	if (len % 2 == 1) {
		return -1;
	}
	unsigned char hb;
	unsigned char lb;
	unsigned int i = 0;
	for (i = 0; i < len; i += 2) {
		if (from[i] >= '0' && from[i] <= '9') {
			hb = from[i] - '0';
		} else if (from[i] >= 'A' && from[i] <= 'F')
			hb = from[i] + 10 - 'A';
		else if (from[i] >= 'a' && from[i] <= 'f')
			hb = from[i] - 'a' + 10;
		else
			return -1;
		if (from[i + 1] >= '0' && from[i + 1] <= '9')
			lb = from[i + 1] - '0';
		else if (from[i + 1] >= 'A' && from[i + 1] <= 'F')
			lb = from[i + 1] + 10 - 'A';
		else if (from[i + 1] >= 'a' && from[i + 1] <= 'f')
			lb = from[i + 1] + 10 - 'a';
		else
			return -1;
		to[i / 2] = (hb << 4) | lb;
	}
	return 0;
}

/*********************************************************************************************
 *函   数   名：ASCII字符串转十六进制字符串
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：
 *修 改  记 录：NONE
 *********************************************************************************************/
int AsciiToHex(unsigned char *to, unsigned char *from, unsigned int len) {
	unsigned char a[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			'a', 'b', 'c', 'd', 'e', 'f' };
	unsigned int i = 0;
	for (i = 0; i < len; i++) {
		to[i * 2] = a[(from[i] & 0xf0) >> 4];
		to[i * 2 + 1] = a[(from[i] & 0x0f)];
	}
	return 0;
}

/********************************************************************/
/*																	*/
/* Function name : GetAppDir										*/
/* Description   : Get application directory.						*/
/*																	*/
/********************************************************************/
void GetAppDir(CString& strAppDir)
{
	TCHAR	szFullPath[MAX_PATH];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];

	// Get application's full path.
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);

	// Break full path into seperate components.
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);

	// Store application's drive and path
	strAppDir.Format(_T("%s%s"), szDrive, szDir);
}