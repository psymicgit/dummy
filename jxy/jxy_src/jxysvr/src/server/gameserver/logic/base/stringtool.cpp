#include "stringtool.h"
using namespace SGDP;

void StringTool::SDSplitStringToUInt(std::vector<UINT32>& vUIntSplit, const std::string& strSrc, CHAR delim) {
	vUIntSplit.clear();

	std::vector<string> vSplitStr = SDSplitStringsA(strSrc, delim);
	for (size_t n = 0; n < vSplitStr.size(); ++n) {
		vUIntSplit.push_back((UINT32)(atoi(vSplitStr[n].c_str())));
	}
}


void StringTool::SDSplitStringToSet( set<UINT16>& setT, const string& strValue, CHAR strSplit)
{
	setT.clear();
	std::vector<string> vSplitStr = SDSplitStringsA(strValue, strSplit);
	for (size_t n = 0; n < vSplitStr.size(); ++n) {
		setT.insert((SDAtoi(vSplitStr[n].c_str())));
	}
}