#pragma once
#include <rapidxml/rapidxml_utils.hpp>
#include <basic/Var.h>

// �� - ��������
struct ClassProperty {
	std::string		propertyName;	// ���� - ������
	VariableType	propertyType;	// ���� - ��������
	bool			save;			// ���� - �Ƿ񱣴浽���ݿ�
	bool			isPublic;		// ���� - �Ƿ��п���
	bool			isPrivate;		// ���� - �Ƿ�˽�п���
	std::string		comment;		// ���� - ע��
};

// �� - ���е���
struct ClassTableColumn {
	VariableType	propertyType;	// �� - ��������
	std::string		comment;		// �� - ע��
};

// �� - ����
struct ClassTable {
	std::string						tableName;		// �� - ����
	bool							save;			// �� - �Ƿ񱣴浽���ݿ�
	bool							isPublic;		// �� - �Ƿ��п���
	bool							isPrivate;		// �� - �Ƿ�˽�п���
	std::string						comment;		// �� - ע��
	std::vector<ClassTableColumn>	vecColumns;		// �� - ���и���
};

// ��
struct OneClass {
	std::vector<ClassProperty>		vecProperty;	// �� - ������
	std::vector<ClassTable>			vecTable;		// �� - ����
	std::string						comment;		// �� - ע��
};

// �����������
struct ClassConfig {
	std::vector<OneClass>			vecClass;
};

// ��ģ��
class ClassModule : public Singleton<ClassModule>
{
public:
	bool Init();

	bool LoadConfig();

	bool LoadClass(const char* path, OneClass& oneClass);
	
	bool ParseClassProperty(rapidxml::xml_node<char> &propertyNode, ClassProperty& classProperty);

	bool ParseClassTable(rapidxml::xml_node<char> &tableNode, ClassTable& classTable);

public:
	static ClassConfig m_config;
};