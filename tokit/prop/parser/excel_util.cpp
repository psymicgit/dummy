///<------------------------------------------------------------------------------
//< @file:   prop\parser\excel_util.cpp
//< @author: hongkunan
//< @date:   2014年4月5日 17:59:55
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include <afxwin.h>

#include "excel_util.h"

#include "file_util.h"

#include "CApplication.h"  
#include "CRange.h"  
#include "CWorkbook.h"  
#include "CWorkbooks.h"  
#include "CWorksheet.h"
#include "CWorksheets.h"

#include "Resource.h"

namespace excelutil
{
    bool init_ole()
    {
        if (!AfxOleInit()){
            AfxMessageBox(IDP_OLE_INIT_FAILED);
            return false;
        }

        if (CoInitialize(NULL)!=0) { 
            AfxMessageBox("初始化COM支持库失败!"); 
            return false;
        }

        return true;
    }

	bool open_sheets_of_excel(string &xlsx, CApplication &ExcelApp, CWorkbooks &books, CWorkbook &book, CWorksheets &sheets)
	{
        if(!init_ole()){
            return false;
        }

		if(false == fileuitl::exist(xlsx))
		{
			std::string err = xlsx + "文件不存在!";
			AfxMessageBox(_T(err.c_str()));
			return false;
		}

		CRange range;
		LPDISPATCH lpDisp = NULL;

		//创建Excel 服务器(启动Excel)
		if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
		{
			AfxMessageBox(_T("启动Excel服务器失败!"));
			return false;
		}

		/*判断当前Excel的版本*/
		CString strExcelVersion = ExcelApp.get_Version();
		int iStart = 0;
		strExcelVersion = strExcelVersion.Tokenize(_T("."), iStart);
		if (_T("11") == strExcelVersion)
		{
			AfxMessageBox(_T("当前Excel的版本是2003。"));
		}
		else if (_T("12") == strExcelVersion)
		{
			// AfxMessageBox(_T("当前Excel的版本是2007。"));
		}
		else
		{
			AfxMessageBox(_T("当前Excel的版本是其他版本。"));
		}

		// ExcelApp.put_Visible(TRUE);
		// ExcelApp.put_UserControl(FALSE);

		/*得到工作簿容器*/
		books.AttachDispatch(ExcelApp.get_Workbooks());

		/*打开一个工作簿，如不存在，则新增一个工作簿*/
		CString strBookPath = _T(xlsx.c_str());
		try
		{
			/*打开一个工作簿*/
			lpDisp = books.Open(strBookPath, 
				vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
				vtMissing, vtMissing, vtMissing, vtMissing, vtMissing, 
				vtMissing, vtMissing, vtMissing, vtMissing);
			book.AttachDispatch(lpDisp);
		}
		catch(...)
		{
			/*增加一个新的工作簿*/
			lpDisp = books.Add(vtMissing);
			book.AttachDispatch(lpDisp);
		}

		/*得到工作簿中的Sheet的容器*/
		sheets.AttachDispatch(book.get_Sheets());
		return true;
	}

	void pre_load_sheet(CWorksheet &sheet, COleSafeArray &safe_array)
	{
		CRange used_range = sheet.get_UsedRange();    

		VARIANT ret_ary = used_range.get_Value2();
		if (!(ret_ary.vt & VT_ARRAY))
		{
			return;
		}
		//
		safe_array.Clear();
		safe_array.Attach(ret_ary); 
	}

	std::string get_cell_str(COleSafeArray &ole_safe_array, int row, int col)
	{
		COleVariant vResult ;
		CString str;

		//字符串
		long read_address[2];
		VARIANT val;
		read_address[0] = row;
		read_address[1] = col;
		ole_safe_array.GetElement(read_address, &val);
		vResult = val;

		if(vResult.vt == VT_BSTR)
		{
			str=vResult.bstrVal;
		}
		//整数
		else if (vResult.vt==VT_INT)
		{
			str.Format("%d",vResult.pintVal);
		}
		//8字节的数字 
		else if (vResult.vt==VT_R8)     
		{
			str.Format("%0.0f",vResult.dblVal);
		}
		//时间格式
		else if(vResult.vt==VT_DATE)    
		{
			SYSTEMTIME st;
			VariantTimeToSystemTime(vResult.date, &st);
			CTime tm(st); 
			str=tm.Format("%Y-%m-%d");
		}
		//单元格空的
		else if(vResult.vt==VT_EMPTY)   
		{
			str="";
		}  

		return str.GetString();
	}

	int get_cell_int(COleSafeArray &ole_safe_array, int row, int col)
	{
		COleVariant vResult ;
		int ret;

		//字符串
		long read_address[2];
		VARIANT val;
		read_address[0] = row;
		read_address[1] = col;
		ole_safe_array.GetElement(read_address, &val);
		vResult = val;

		//整数
		switch(vResult.vt)
		{
		case VT_INT:
			ret = vResult.intVal;
			break;

		case VT_R8:
			ret = (int)vResult.dblVal;
			break;

		default:
			ret = 0;
			break;
		}

		return ret;
	}

	int get_row_cnt(CWorksheet &sheet)
	{
		CRange range;
		CRange usedRange;

		usedRange.AttachDispatch(sheet.get_UsedRange(), true);
		range.AttachDispatch(usedRange.get_Rows(), true);
		int row_cnt = range.get_Count();
		usedRange.ReleaseDispatch();
		range.ReleaseDispatch();
		return row_cnt;
	}

	int get_col_cnt(CWorksheet &sheet)
	{
		CRange range;
		CRange usedRange;

		usedRange.AttachDispatch(sheet.get_UsedRange(), true);
		range.AttachDispatch(usedRange.get_Columns(), true);
		int count = range.get_Count();
		usedRange.ReleaseDispatch();
		range.ReleaseDispatch();
		return count;
	}
}