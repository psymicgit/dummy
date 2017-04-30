//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年4月30日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

enum VarType
{
	VarType_Int,
	VarType_Int64,
	VarType_Double,
	VarType_String,
};

class VarList
{
	int Size();

	int IntVal();
	int Int64Val();
	int DoubleVal();
	int StringVal();
};