#include "sdcsvfile.h"
#include <iostream>
#include <fstream>
#include "sdstring.h"
namespace SGDP 
{
	INT32 CSDCsvBase::GetLineCount()
	{
		return ((INT32)m_vLine.size());
	}

	INT32 CSDCsvBase::GetItemCount( INT32 line )
	{
		if ((INT32)m_vLine.size() <= line)
		{
			return -1;
		}

		return (INT32)m_vLine[line].size();
	}

	INT32 CSDCsvBase::InsertLine(INT32 line)
	{
		std::vector<tstring> vec;
		if (line >= (INT32)m_vLine.size())
		{
			//line too big, insert more lines
			INT32 oldsize = (INT32)m_vLine.size();
			INT32 dif = line + 1 - oldsize;
			for(INT32 i=0; i<dif; i++)
			{
				m_vLine.push_back(vec);
			}
		}
		else
		{
			//insert one line in the "line" place
			m_vLine.insert(m_vLine.begin()+line, vec);
		}
		return ((INT32)m_vLine.size());
	}

	INT32 CSDCsvBase::AddLine()
	{
		std::vector<tstring> vec;
		m_vLine.insert(m_vLine.begin()+m_vLine.size(), vec);
		return ((INT32)m_vLine.size());
	}

	INT32 CSDCsvBase::InsertItem(INT32 line, INT32 item, const TCHAR *data)
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}

		std::vector<tstring> & itemvec = m_vLine[line];
		tstring str = data;
		tstring s = _SDT("");
		if (item >= (INT32)itemvec.size())
		{
			//item too big, insert some empty string
			INT32 dif = item - (INT32)itemvec.size();
			for (INT32 i=0; i<dif; i++)
			{
				itemvec.push_back(s);
			}
			itemvec.push_back(str);
		}
		else
		{
			//insert string in the "item" place
			itemvec.insert(itemvec.begin()+item, str);
		}

		return ((INT32)itemvec.size());
	}

	INT32 CSDCsvBase::DelItem(INT32 line, INT32 item)
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}

		std::vector<tstring> &itemvec = m_vLine[line];

		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		itemvec.erase(itemvec.begin()+item);

		return ((INT32)itemvec.size());
	}

	INT32 CSDCsvBase::DelLine(INT32 line)
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}

		std::vector<tstring> &itemvec = m_vLine[line];
		itemvec.clear();
		m_vLine.erase(m_vLine.begin()+line+1);

		return ((INT32)itemvec.size());
	}

	INT32 CSDCsvBase::ModifyItem(INT32 line, INT32 item, const TCHAR * data)
	{
		if (line >= (INT32)m_vLine.size())
		{
			return -1;
		}

		std::vector<tstring> &itemvec = m_vLine[line];

		if (item >= (INT32)itemvec.size())
		{
			return -1;
		}

		itemvec[item] = data;

		return ((INT32)itemvec.size());
	}

	INT32 CSDCsvBase::EmptyItem(INT32 line, INT32 item)
	{
		return ModifyItem(line, item, _SDT(""));
	}

	INT32 CSDCsvBase::ReadData( INT32 line, INT32 item, TCHAR *data, INT32 size )
	{
		//get line
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
		if (size <= (INT32)itemvec[item].size())
		{
			size = (INT32)itemvec[item].size() - 1;
		}
		else
		{
			size = (INT32)itemvec[item].size();
		}

		memset(data, 0, size*sizeof(TCHAR));
		memcpy(data, itemvec[item].c_str(), size*sizeof(TCHAR));
		data[size] = 0;
		return ((INT32)itemvec.size()-item-1);
	}

	INT32 CSDCsvString::Init( const tstring& strData )
	{
		tstring  strSubData;
		tstring  strTemp;
		INT32   nStart  = 0;
		INT32   nEnd    = 0;
		INT32   nSubStart  = 0;
		INT32   nSubEnd  = 0;
		nStart    = (INT32)strData.find(_SDT("{"), nStart);
		std::vector<tstring> vecItem;
		while (nStart != strData.npos)
		{
			vecItem.resize(0);
			nStart ++;
			nEnd = (INT32)strData.find(_SDT("}"), nStart);
			if (nEnd != strData.npos)
			{
				strSubData = strData.substr(nStart,nEnd - nStart);
				if (strSubData.length() > 0)
				{
					nSubStart = 0;
					do 
					{
						nSubEnd  = (INT32)strSubData.find(_SDT(","), nSubStart);
						strTemp = strSubData.substr(nSubStart,nSubEnd - nSubStart);
						vecItem.push_back(strTemp);
						nSubStart = nSubEnd + 1;
					} while (nSubEnd != strSubData.npos);
				}   
			}
			else
			{
				return -1;
			}
			m_vLine.push_back(vecItem);
			nStart  = (INT32)strData.find(_SDT("{"), nEnd);
		}
		return ((INT32)m_vLine.size());
	}

	INT32 CSDCsvString::FlushData( tstring& strOutPut )
	{
		return 0;
	}

	INT32 CSDCsvfile::Init(const TCHAR *path)
	{
		try
		{
			std::locale loc = std::locale::global(std::locale(""));

			std::ifstream in;
			in.open(_SDTT2A(path));
			if(!in)
			{
				return -1;
			}
			in.seekg(4);
			while(!in.eof())
			{
				std::string linestr; 
				getline(in, linestr);
				if (linestr.empty())
				{
					continue;
				}

				//item vector, a item is one string
				std::vector<tstring> itemvec;
				//a item string start pos
				INT32 start = 0;
				//a item string end pos
				INT32 end = 0;
				while(true)
				{
					//item string
					std::string itemstr;
					//string begin has "
					if (linestr[start] == '"')
					{
						end = (INT32)linestr.find("\"", end+1);
						if (end == std::string::npos)
						{
							itemstr = linestr.substr(start);
							itemvec.push_back(_SDTA2T(itemstr.c_str()));
							break;
						}
						//find a match ", erase them from string
						else
						{
							start++;
							linestr.erase(end, 1);
						}
					}
					//find ","
					end = (INT32)linestr.find(",", end);
					if (end == std::string::npos)
					{
						itemstr = linestr.substr(start);
						itemvec.push_back(_SDTA2T(itemstr.c_str()));
						break;
					}
					itemstr = linestr.substr(start, end-start);
					itemvec.push_back(_SDTA2T(itemstr.c_str()));
					start = ++end;
				}
				m_vLine.push_back(itemvec);
			}
			in.close();

			std::locale::global(loc);
		}
		catch (const std::exception& err)
		{//ifstream open file may throw ios_base::failure exception
			std::cerr << err.what() << std::endl;
			return -1;
		}
		m_strFilePath = path;
		return ((INT32)m_vLine.size());
	}

	INT32 CSDCsvfile::FlushData()
	{
		if (m_strFilePath.empty())
		{
			return -1;
		}
		try
		{
			std::ofstream out;
			out.open(_SDTT2A(m_strFilePath.c_str()), std::ios_base::trunc);
			if (!out)
			{
				return -1;
			}
			CHAR szBOM[4] = {(CHAR)0xEF, (CHAR)0xBB, (CHAR)0xBF, 0};   
			out << szBOM[3] << szBOM[2] << szBOM[1] << szBOM[0];
			for (INT32 i=0; i < (INT32)m_vLine.size(); i++)
			{
				std::vector<tstring> &itemvec = m_vLine[i];

				INT32 j=0;
				for (j=0; j < (INT32)itemvec.size()-1; j++)
				{
					tstring &itemstr = itemvec[j];
					INT32 indexch;
					if (indexch = itemstr.find(_SDT(",")) == tstring::npos)
					{
						out << "\"" << _SDTT2A(itemstr.c_str()) << "\"" << ",";
					}
					else
					{
						out << _SDTT2A(itemstr.c_str()) << ",";
					}
				}
				tstring &itemstr = itemvec[j];
				INT32 indexch;
				if (indexch = (INT32)itemstr.find(_SDT(",")) == std::string::npos)
				{
					out << "\"" << _SDTT2A(itemstr.c_str()) << "\"" << "\n";
				}
				else
				{
					out << _SDTT2A(itemstr.c_str()) << "\n";
				}
			}
			out.close();
		}
		catch (const std::exception& err)
		{//ifstream open file may throw ios_base::failure exception
			std::cerr << err.what() << std::endl;
			return -1;
		}
		return 1;
	}

	INT32 CSDCsvfile::ReadCsvString( INT32 line, INT32 item, CSDCsvString& obCsvString )
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
		obCsvString.Clear();
		return obCsvString.Init(itemvec[item]);
	}

}

