#include "Var.h"

// 变量类型是否合法
bool IsVariableTypeValid(VariableType varType)
{
	return VariableType_None < varType && varType < VariableType_Max;
}

VariableType ParseVariableType(const char* text)
{
	const std::string str(text);

	int n = sizeof(VariableTypeText) / sizeof(VariableTypeText[0]);

	for (int i = 0; i < n; ++i)
	{
		if (str == VariableTypeText[i])
		{
			return (VariableType)(i + 1);
		}
	}

	return VariableType_None;
}

VarList::VarList()
{
	Init();
}

VarList& VarList::operator<<(int num)
{
	PushInt(num);
	return *this;
}

VarList& VarList::operator<<(int64_t num)
{
	PushInt64(num);
	return *this;
}

VarList& VarList::operator<<(float num)
{
	PushFloat(num);
	return *this;
}

VarList& VarList::operator<<(double num)
{
	PushDouble(num);
	return *this;
}

VarList& VarList::operator<<(const char* text)
{
	PushString(text);
	return *this;
}

VarList& VarList::operator<<(const wchar_t* text)
{
	PushWideString(text);
	return *this;
}

VarList& VarList::operator<<(const std::string& text)
{
	PushString(text.c_str());
	return *this;
}

VarList& VarList::operator<<(const std::wstring& text)
{
	PushWideString(text.c_str());
	return *this;
}

VarList& VarList::operator<<(const VarList& other)
{

}

void VarList::Clear()
{
	m_elementCount = 0;
	m_bufferUsed = 0;
}

int VarList::Size()
{
	return m_elementCount;
}

int VarList::IntVal(int slot)
{
	if (slot < 0 || slot > m_elementCount)
	{
		return 0;
	}

	const VarElement& element = m_elements[slot];
	switch(element.varType)
	{
	case VariableTypeText_Int:
		return element.intVal;
		break;
		
	case VariableTypeText_Int64:
		return (int)element.int64Val;
		break;
		
	case VariableTypeText_Float:
		return (int)element.floatVal;
		break;
		
	case VariableTypeText_Double:
		return (int)element.doubleVal;
	}
	
	return 0;
}

int64_t VarList::Int64Val(int slot)
{
	if (slot < 0 || slot > m_elementCount)
	{
		return 0;
	}

	const VarElement& element = m_elements[slot];
	switch(element.varType)
	{
	case VariableTypeText_Int:
		return (int64_t)element.intVal;
		break;
		
	case VariableTypeText_Int64:
		return element.int64Val;
		
	case VariableTypeText_Float:
		return (int64_t)element.floatVal;
		
	case VariableTypeText_Double:
		return (int64_t)element.doubleVal;
	}
	
	return 0;
}

float VarList::FloatVal(int slot)
{
	if (slot < 0 || slot > m_elementCount)
	{
		return 0.0f;
	}

	const VarElement& element = m_elements[slot];
	switch(element.varType)
	{
	case VariableTypeText_Int:
		return (float)element.intVal;
		break;
		
	case VariableTypeText_Int64:
		return (float)element.int64Val;
		
	case VariableTypeText_Float:
		return (float)element.floatVal;
		
	case VariableTypeText_Double:
		return (float)element.doubleVal;
	}
	
	return 0.0f;
}

double VarList::DoubleVal(int slot)
{
	if (slot < 0 || slot > m_elementCount)
	{
		return 0.0f;
	}

	const VarElement& element = m_elements[slot];
	switch(element.varType)
	{
	case VariableTypeText_Int:
		return (double)element.intVal;
		break;
		
	case VariableTypeText_Int64:
		return (double)element.int64Val;
		
	case VariableTypeText_Float:
		return (double)element.floatVal;
		
	case VariableTypeText_Double:
		return element.doubleVal;
	}
	
	return 0.0f;
}

const char* VarList::StringVal(int slot)
{
	if (slot < 0 || slot > m_elementCount)
	{
		return "";
	}

	const VarElement& element = m_elements[slot];
	switch(element.varType)
	{
	case VariableTypeText_String:
		return element.stringVal;

	default:
		break;
	}
	
	return "";
}

const wchar_t* VarList::WideStrVal(int slot)
{
	if (slot < 0 || slot > m_elementCount)
	{
		return L"";
	}

	const VarElement& element = m_elements[slot];
	switch(element.varType)
	{
	case VariableTypeText_WideString:
		return element.widestrVal;

	default:
		break;
	}
	
	return L"";
}

bool VarList::Init()
{
	m_elements = m_elementsAtStack;
	m_elementCapacity = sizeof(m_elementsAtStack) / sizeof(m_elementsAtStack[0]);
	m_elementCount = 0;

	m_buffer = m_bufferAtStack;
	m_bufferSize = sizeof(m_bufferAtStack);
	m_bufferUsed = 0;

	return true;
}

VarElement& VarList::PushElement()
{
	ExpandElement(1);

	int before = m_elementCount;
	++m_elementCount;
	return m_elements[before];
}

void VarList::ExpandElement(int add)
{
	int after = m_elementCount + add;

	// 空间不够时，在堆上申请内存
	if (after > m_elementCapacity)
	{
		// 内存2倍速增长
		int finalCapacity = m_elementCapacity * 2;

		while (finalCapacity < after)
		{
			finalCapacity *= 2;
		}

		VarElement* finalElements = new VarElement[finalCapacity];
		memmove_s(finalElements, finalCapacity, m_elements, m_elementCount);

		delete[] m_elements;
		m_elements = finalElements;
	}
}

void VarList::ExpandBuffer(int add)
{
	int after = m_bufferUsed + add;

	// 空间不够时，在堆上申请内存
	if (after > m_bufferSize)
	{
		// 内存2倍速增长
		int finalSize = m_bufferSize * 2;

		while (finalSize < after)
		{
			finalSize *= 2;
		}

		char* finalBuffer = new char[finalSize];
		memmove_s(finalBuffer, finalSize, m_buffer, m_bufferUsed);

		delete[] m_buffer;
		m_buffer = finalBuffer;
	}
}

void VarList::PushText(const char* at, int size)
{
	ExpandBuffer(size);
	memcpy_s(m_buffer + m_bufferUsed, size, at, size);
	m_bufferUsed += size;
}

void VarList::PushInt(int num)
{
	VarElement& element = PushElement();
	element.varType = VariableType_Int;
	element.intVal = num;
}

void VarList::PushInt64(int64_t num)
{
	VarElement& element = PushElement();
	element.varType = VariableType_Int64;
	element.int64Val = num;
}

void VarList::PushFloat(float num)
{
	VarElement& element = PushElement();
	element.varType = VariableType_Float;
	element.floatVal = num;
}

void VarList::PushDouble(double num)
{
	VarElement& element = PushElement();
	element.varType = VariableType_Double;
	element.doubleVal = num;
}

void VarList::PushString(const char* text)
{
	int before = m_bufferUsed;

	int size = strlen(text);
	PushText(text, size + 1);
	m_buffer[size] = '\0';

	VarElement& element = PushElement();
	element.varType = VariableType_String;
	element.stringStartPos = before;
}

void VarList::PushWideString(const wchar_t* text)
{
	int before = m_bufferUsed;

	int size = wcslen(text) * sizeof(wchar_t);
	PushText((const char*)text, size + 2);
	m_buffer[size] = '\0';
	m_buffer[size + 1] = '\0';

	VarElement& element = PushElement();
	element.varType = VariableType_WideString;
	element.widestrStartPos = before;
}

void VarList::PushVarList(const VarList& other)
{
	if (other.m_elementCount > 0)
	{
		PushVarListBetween(other, 0, other.m_elementCount - 1);
	}
}

void VarList::PushVarListBetween(const VarList& other, int from, int to)
{
	// 检查：不能越界
	if (from < 0 || to >= other.m_elementCount || to < from)
	{
		return;
	}

	// 预分配空间
	int copy = to - from + 1;
	ExpandElement(copy);

	// 拷贝元素
	for (int i = from; i <= to; ++i)
	{
		PushVarElement(other.m_elements[i]);
	}
}

void VarList::PushVarElement(const VarElement& element)
{
	switch (element.varType)
	{
	case VariableType_Int:
	case VariableType_Int64:
	case VariableType_Float:
	case VariableType_Double:
	{
		VarElement& newElement = PushElement();
		newElement = element;
	}
	break;

	case VariableType_String:
	{
		PushString(element.m_buffer + element.stringStartPos);
	}
	break;
	
	case VariableType_WideString:
	{
		PushWideString((const wchar_t*)element.m_buffer + element.widestrStartPos);
	}
	break;

	default:
		break;
	}
}

void VarList::Test()
{
	VarList var;
		
	int n = 100;

	for (int i = 1; i <= 100; ++i)
	{
		int intVal = i;
		int64_t int64Val = (int64_t)i;
		float floatVal = (int64_t)i;
		double doubleVal = (double)i;
		std::string stringVal = strtool::itoa(i);
		std::wstring wstringVal = strtool::s2ws(stringVal);
		
		var << intVal << int64Val << floatVal << doubleVal << stringVal << wstringVal;
		
		int slot = i * 6;
		
		bool ok = true;
		ok &= (var.IntVal(slot) == intVal);
		ok &= (var.Int64Val(slot + 1) == int64Val);
		ok &= (var.FloatVal(slot + 2) == floatVal);
		ok &= (var.DoubleVal(slot + 3) == doubleVal);
		ok &= (var.StringVal(slot + 4) == stringVal);
		ok &= (var.WStringVal(slot + 5) == wstringVal);
		
		if (!ok)
		{
			break;
		}
	}
}