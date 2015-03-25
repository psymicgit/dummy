using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using Microsoft.Office.Interop.Excel;
using Microsoft.Office.Interop;
using System.IO;

namespace SaveExcelAsXml
{
    class Program
    {
        // 根据文件内容解析出文件名
        // 如下：
        // <?xml version="1.0" standalone="yes"?>
        // <EquipMasterRates>
        //	  <EquipMasterRate EquipKind="0" Rank="0" Level="0" Promote="0"/>
        // 则最后取出EquipMasterRate
        static string parse_filename(string xmldata)
        {
            int occur = 0;
            int i = 0;
            for (; i < xmldata.Length; i++)
            {
                if (xmldata[i] == '<')
                {
                    ++occur;
                    if (occur == 3)
                    {
                        break;
                    }
                }
            }

            int end_of_name = i;
            for (; end_of_name < xmldata.Length; end_of_name++)
            {
                if (xmldata[end_of_name] == ' ')
                {
                    break;
                }
            }

            string filename = xmldata.Substring(i + 1, end_of_name - i - 1);
            return filename;
        }

        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("传入的参数错误，格式应为:SaveExcelAsXml xlsx文件路径 xml文件放到哪个路径");
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
            
            string xlsx = args[0];
            string xml_dir  = args[1];

            if(false == File.Exists(xlsx))
            {
                Console.WriteLine("{0}文件不存在", xlsx);
                return;
            }

            Microsoft.Office.Interop.Excel.Application xApp = new Microsoft.Office.Interop.Excel.Application();

            Workbook excelWorkBook = (Workbook)xApp.Workbooks.Open(xlsx,
                   Type.Missing, Type.Missing, Type.Missing, Type.Missing,
                   Type.Missing, Type.Missing, Type.Missing, Type.Missing,
                   Type.Missing, Type.Missing, Type.Missing, Type.Missing,
                   Type.Missing, Type.Missing);


            xApp.Visible = false;
            
            string csv = xlsx.Replace(".xlsx", ".csv");
            excelWorkBook.SaveAs(csv, XlFileFormat.xlCSV);
            excelWorkBook.Close();

            Console.WriteLine("保存{0}到{1}成功!", xlsx, csv);
        }
    }
}
