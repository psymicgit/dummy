#pragma once
#include <rapidxml/rapidxml_utils.hpp>
#include <basic/Var.h>

// 类 - 单个属性
struct ClassProperty {
	std::string		propertyName;	// 属性 - 属性名
	VariableType	propertyType;	// 属性 - 属性类型
	bool			save;			// 属性 - 是否保存到数据库
	bool			isPublic;		// 属性 - 是否公有可视
	bool			isPrivate;		// 属性 - 是否私有可视
	std::string		comment;		// 属性 - 注释
};

// 类 - 表中单列
struct ClassTableColumn {
	VariableType	propertyType;	// 列 - 属性类型
	std::string		comment;		// 列 - 注释
};

// 类 - 单表
struct ClassTable {
	std::string						tableName;		// 表 - 表名
	bool							save;			// 表 - 是否保存到数据库
	bool							isPublic;		// 表 - 是否公有可视
	bool							isPrivate;		// 表 - 是否私有可视
	std::string						comment;		// 表 - 注释
	std::vector<ClassTableColumn>	vecColumns;		// 表 - 表中各列
};

// 类
struct OneClass {
	std::vector<ClassProperty>		vecProperty;	// 类 - 各属性
	std::vector<ClassTable>			vecTable;		// 类 - 各表
	std::string						comment;		// 类 - 注释
};

// 所有类的配置
struct ClassConfig {
	std::vector<OneClass>			vecClass;
};

// 类模块
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