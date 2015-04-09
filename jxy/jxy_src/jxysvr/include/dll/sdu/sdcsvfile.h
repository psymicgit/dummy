#ifndef __SD_CSVFILE_H_
#define __SD_CSVFILE_H_
#include "sdtype.h"
#include <vector>
#include <string>
#include "sdstring.h"
using namespace std;
namespace SGDP 
{
    /**
    * @brief 处理类似下面的字符串矩阵
    *        falfj     afljslf       aldfj
             alfdlasf  oeeofljdfls   adljldo   sdf;sdfk
             aldjl     aldfjolsdf    ljoewrjjls
    */
	class  CSDCsvBase
	{
	public:

		/**
		* @brief 构造函数
		*/
		CSDCsvBase(){}

		/**
		* @brief 析构函数
		*/
		~CSDCsvBase(){}

        /**
        * @brief 清空容器，重置容器m_vLine大小为0
        * @return VOID
        */
		VOID    Clear()
        {
            m_vLine.resize(0);
        }

        /*
        * @brief 获得行的个数
        * @return 行数
        */
		INT32 GetLineCount();

        /**
        * @brief 获得指定行的列数
        * @param line : 指定行
        * @return 列数
        */
		INT32 GetItemCount(INT32 line);

        /**
        * @brief 在指定行插入一个空行
        * @param line : 指定行
        * @return 插入后容器的大小
        */
		INT32 InsertLine(INT32 line);

        /**
        * @brief 在容器尾部插入一个空行
        * @return 插入后容器的大小
        */
		INT32 AddLine();

        /**
        * @brief 在指定行、指定列中插入一项数据
        * @param line : 指定行
        * @param item : 指定列
        * @param data : 待插入的数据
        * @return 插入后当前行的列数
        */
		INT32 InsertItem(INT32 line, INT32 item, const TCHAR *data);

		/**
        * @brief 删除指定行、指定列
        * @param line : 指定行
        * @param item : 指定列
        * @return 成功则返回删除后当前行的列数，否则返回-1
        */
		INT32 DelItem(INT32 line, INT32 item);

		/**
        * @brief 删除指定行
        * @param line : 指定行
        * @return 成功则返回删除后容器的行数，否则返回-1
        */
		INT32 DelLine(INT32 line);

		/**
        * @brief 重置指定行、指定列的数据为""
        * @param line : 指定行
        * @param item : 指定列
        * @return 成功则返回重置后当前行的列数，否则返回-1
        */
		INT32 EmptyItem(INT32 line, INT32 item);

		/**
        * @brief 修改指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param data : 修改后的数据
        * @return 成功则返回重置后当前行的列数，否则返回-1
        */
		INT32 ModifyItem(INT32 line, INT32 item, const TCHAR *data);

		/**
        * @brief 获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param data : 输出取到的数据
        * @param size : 指定行、指定列的数据长度
        * @return 成功则返回剩余的列个数，否则返回-1
        */
		INT32 ReadData(INT32 line, INT32 item, TCHAR *data, INT32 size);

        /**
        * @brief 以下根据T*类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, T* obItem);

		/**
        * @brief 以下根据T&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, T& obItem);

		/**
        * @brief 以下根据TCHAR&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, TCHAR& cItem);

		/**
        * @brief 以下根据INT8&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, UINT8& byItem);

		/**
        * @brief 以下根据INT16&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, INT16& shItem);

		/**
        * @brief 以下根据UINT16&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, UINT16& wItem);

		/**
        * @brief 以下根据TINT32&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, INT32& nItem);

		/**
        * @brief 以下根据UINT32&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, UINT32& dwItem);

		/**
        * @brief 以下根据float&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, float& fItem);

		/**
        * @brief 以下根据double&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, double& fItem);

		/**
        * @brief 以下根据tstring&类型获取指定行、指定列的数据
        * @param line : 指定行
        * @param item : 指定列
        * @param ?+Item : 保存取到的不同类型数据
        * @return 成功则返回0，否则返回-1
        */
		template <typename T>
		INT32 ReadData(INT32 line, INT32 item, tstring& strItem);

	protected:
		std::vector<std::vector<tstring> > m_vLine;
	private:
	};

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, T& obItem )
	{
		return -1;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, T* obItem )
	{
		return -1;  
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, TCHAR& cItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<tstring> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		cItem = _SDTTtoi(itemvec[item].c_str());
		return 0;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, UINT8& byItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<tstring> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		byItem = atoi(_SDTT2A(itemvec[item].c_str()));
		return 0;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, INT16& shItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<tstring> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		shItem = _SDTTtoi(itemvec[item].c_str());
		return 0;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, UINT16& wItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<tstring> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		wItem = _SDTTtoi(itemvec[item].c_str());
		return 0;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, INT32& nItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<tstring> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		nItem = _SDTTtoi(itemvec[item].c_str());
		return 0;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, UINT32& dwItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<tstring> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		dwItem = _SDTTtoi(itemvec[item].c_str());
		return 0;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, float& fItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<tstring> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		fItem = static_cast<float>(atof(_SDTT2A(itemvec[item].c_str())));
		return 0;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, double& fItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<tstring> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		fItem = atof(_SDTT2A(itemvec[item].c_str()));
		return 0;
	}

	template <typename T>
	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, tstring& strItem )
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}
		std::vector<std::string> & itemvec = m_vLine[line];
		//get item
		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		strItem = itemvec[item];
		return 0;
	}

	class CSDCsvString:public CSDCsvBase
	{
		/*
		字符串格式:
		{.,..,...,...}...{.,..,...,...}
		*/
	public:

		/**
		* @brief 构造函数
		*/
		CSDCsvString(){}

		/**
		* @brief 析构函数
		*/
		~CSDCsvString(){}

        /**
        * @brief 根据给定格式的字符串初始化容器
        * @param strData : 字符串数据
        * @return 容器的大小
        */
		INT32 Init(const tstring& strData);

        /**
        * @brief 输出字符串数据
        * @return 0
        */
		INT32 FlushData(tstring& strOutPut);
	};

	class CSDCsvfile:public CSDCsvBase
	{
	public:

		/**
		* @brief 构造函数
		*/
		CSDCsvfile(){}

		/**
		* @brief 析构函数
		*/
		~CSDCsvfile(){}

		/**
        * @brief 根据文件路径获取给定格式的字符串，初始化容器
        * @param path : csv文件路径
        * @return 成功则返回容器的大小，否则返回-1
        */
		INT32 Init(const TCHAR *path);

		/**
        * @brief 将容器中的数据输出到文件中
        * @return 成功则返回1，否则返回-1
        */
		INT32 FlushData();

        /**
        * @brief 从容器中指定行、指定列中读取字符串到obCsvString中
        * @param line : 指定行
        * @param item : 指定列
        * @param obCsvString : 保存读取的字符串
        * @return 成功则返回容器大小，否则返回-1
        */
		INT32 ReadCsvString(INT32 line, INT32 item, CSDCsvString& obCsvString);

	private:
		//file path
		tstring m_strFilePath;
	};
}
#endif //#ifndef _CSVFILE_H_

