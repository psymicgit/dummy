//------------------------------------------------------------------------------
// ����: ������
// ����: 2017��4��30��
// ˵��: 
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