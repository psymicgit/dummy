using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using Microsoft.Office.Interop.Excel;
using Microsoft.Office.Interop;
using System.IO;

namespace SaveXmlAsExcel
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("传入的参数错误，格式应为:SaveExcelAsXml xml文件路径 xlsx文件路径");
                if (args.Length > 0)
                {
                    Console.WriteLine("传入的参数为：");
                }

                for (int i = 0; i < args.Length; i++)
                {
                    Console.WriteLine(args[i] + "  ");
                }

                return;
            }

            string xml  = args[0];
            string xlsx = args[1];

            if (false == File.Exists(xml))
            {
                Console.WriteLine("{0}文件不存在", xlsx);
                return;
            }

            Microsoft.Office.Interop.Excel.Application xApp = new Microsoft.Office.Interop.Excel.Application();
            Microsoft.Office.Interop.Excel.Workbook excelWorkBook = xApp.Workbooks.OpenXML(xml, Type.Missing, Microsoft.Office.Interop.Excel.XlXmlLoadOption.xlXmlLoadImportToList);

            Microsoft.Office.Interop.Excel.Worksheet worksheet = excelWorkBook.Sheets[1];
            worksheet.Cells.Font.Size = 10;
            worksheet.Cells.HorizontalAlignment = Microsoft.Office.Interop.Excel.XlHAlign.xlHAlignCenter;
            // Microsoft.Office.Interop.Excel.Range range = worksheet.get_Range(worksheet.Cells[1, 1], worksheet.Cells[11, 3]);
            // range.Font.Bold = true;

            excelWorkBook.SaveAs(xlsx, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Type.Missing, Microsoft.Office.Interop.Excel.XlSaveAsAccessMode.xlNoChange);

            // excelWorkBook.SaveAs(xlsx);
            excelWorkBook.Close();
            xApp.Workbooks.Close();
        }
    }
}
