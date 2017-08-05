#pragma once

// 变量类型
enum VariableType
{
	VariableType_None = 0,	// 无效
	VariableType_Int,
	VariableType_Int64,
	VariableType_Float,
	VariableType_Double,
	VariableType_String,
	VariableType_WideString,
	VariableType_Max
};

// 变量类型名
const static char* const VariableTypeText_Int = "int";
const static char* const VariableTypeText_Int64 = "int64";
const static char* const VariableTypeText_Float = "float";
const static char* const VariableTypeText_Double = "double";
const static char* const VariableTypeText_String = "string";
const static char* const VariableTypeText_WideString = "widestr";

const static char* const VariableTypeText[] =
{
	VariableTypeText_Int,
	VariableTypeText_Int64,
	VariableTypeText_Float,
	VariableTypeText_Double,
	VariableTypeText_String,
	VariableTypeText_WideString
};

// 变量类型是否合法
bool IsVariableTypeValid(VariableType varType);

// 根据字符串获取变量类型
VariableType ParseVariableType(const char* text);

// 元素
struct VarElement
{
	VarElement()
		: varType(VariableType_None)
	{
	}

	VariableType varType;

	union
	{
		int				intVal;
		int64_t			int64Val;
		float			floatVal;
		double			doubleVal;
		int				stringStartPos;
		int				widestrStartPos;
	};	
};

// 变量列表
class VarList
{
public:
	VarList();

public:
	VarList& operator<<(int num);
	VarList& operator<<(int64_t num);
	VarList& operator<<(float num);
	VarList& operator<<(double num);
	VarList& operator<<(const char* text);
	VarList& operator<<(const wchar_t* text);
	VarList& operator<<(const std::string& text);
	VarList& operator<<(const std::wstring& text);
	VarList& operator<<(const VarList& other);

public:
	void Clear();

	int Size();
	
	int IntVal(int slot);
	int64_t Int64Val(int slot);
	float FloatVal(int slot);
	double DoubleVal(int slot);
	const char* StringVal(int slot);
	const wchar_t* WideStrVal(int slot);

private:
	inline bool Init();

	VarElement& PushElement();

	void ExpandElement(int add);
	void ExpandBuffer(int add);

private:
	inline void PushText(const char* at, int size);
	inline void PushInt(int num);
	inline void PushInt64(int64_t num);
	inline void PushFloat(float num);
	inline void PushDouble(double num);
	inline void PushString(const char* text);
	inline void PushWideString(const wchar_t* text);
	inline void PushVarList(const VarList& other);
	inline void PushVarListBetween(const VarList& other, int from, int to);
	inline void PushVarElement(const VarElement& element);

public:
	// 测试用例
	static void Test();

public:
	VarElement		m_elementsAtStack[8];	// 元素 - 预先分配在栈上的缓冲区
	VarElement*		m_elements;				// 元素 - 初始指向栈，空间不够时再指向新申请的堆
	int				m_elementCapacity;		// 元素 - 容量
	int				m_elementCount;			// 元素 - 个数
	char			m_bufferAtStack[128];	// 缓冲区 - 预先分配在栈上的缓冲区
	char*			m_buffer;				// 缓冲区 - 缓冲区，初始指向栈，空间不够时再指向新申请的堆
	int				m_bufferSize;			// 缓冲区 - 容量
	int				m_bufferUsed;			// 缓冲区 - 已使用的大小
};