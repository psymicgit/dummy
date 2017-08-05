#pragma once

// ��������
enum VariableType
{
	VariableType_None = 0,	// ��Ч
	VariableType_Int,
	VariableType_Int64,
	VariableType_Float,
	VariableType_Double,
	VariableType_String,
	VariableType_WideString,
	VariableType_Max
};

// ����������
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

// ���������Ƿ�Ϸ�
bool IsVariableTypeValid(VariableType varType);

// �����ַ�����ȡ��������
VariableType ParseVariableType(const char* text);

// Ԫ��
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

// �����б�
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
	// ��������
	static void Test();

public:
	VarElement		m_elementsAtStack[8];	// Ԫ�� - Ԥ�ȷ�����ջ�ϵĻ�����
	VarElement*		m_elements;				// Ԫ�� - ��ʼָ��ջ���ռ䲻��ʱ��ָ��������Ķ�
	int				m_elementCapacity;		// Ԫ�� - ����
	int				m_elementCount;			// Ԫ�� - ����
	char			m_bufferAtStack[128];	// ������ - Ԥ�ȷ�����ջ�ϵĻ�����
	char*			m_buffer;				// ������ - ����������ʼָ��ջ���ռ䲻��ʱ��ָ��������Ķ�
	int				m_bufferSize;			// ������ - ����
	int				m_bufferUsed;			// ������ - ��ʹ�õĴ�С
};