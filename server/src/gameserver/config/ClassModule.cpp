#include "ClassModule.h"
#include <tool/strtool.h>

ClassConfig ClassModule::m_config;

bool ClassModule::Init()
{
	return true;
}

bool ClassModule::LoadConfig()
{
	return true;
}

bool ClassModule::LoadClass(const char* path, OneClass& oneClass)
{
	// ��xml�ļ�
	rapidxml::file<> xmlFile(path);
	rapidxml::xml_document<> doc;
	if (!xmlFile.data())
	{
		LOG_ERROR << "load class from file <" << path << "> failed!\n";
		return false;
	}

	doc.parse<0>(xmlFile.data());

	// root���ڵ�
	rapidxml::xml_node<>* root = doc.first_node("root");
	if (!root)
	{
		LOG_ERROR << "load class from file <" << path << "> failed! not found <root> node\n";
		return false;
	}

	// root���ڵ� -> propertys�ڵ�
	rapidxml::xml_node<>* propertysNode = root->first_node("propertys");
	if (!propertysNode)
	{
		LOG_ERROR << "load class from file <" << path << "> failed! not found <propertys> node\n";
		return false;
	}

	// root���ڵ� -> tables�ڵ�
	rapidxml::xml_node<>* tablesNode = root->first_node("tables");
	if (!tablesNode)
	{
		LOG_ERROR << "load class from file <" << path << "> failed! not found <tables> node\n";
		return false;
	}

	// ������root���ڵ� -> propertys�ڵ� -> property�ڵ��б�
	for (rapidxml::xml_node<>* propertyNode = propertysNode->first_node("property"); propertyNode; propertyNode = propertyNode->next_sibling("property"))
	{
		ClassProperty classProperty;

		if (!ParseClassProperty(*propertyNode, classProperty))
		{
			LOG_ERROR << "load class from file <" << path << "> failed! <" << propertyNode->contents() << "> node is invalid!";
			return false;
		}

		oneClass.vecProperty.push_back(classProperty);
	}

	// ������root���ڵ� -> tables�ڵ� -> table�ڵ��б�
	for (rapidxml::xml_node<>* tableNode = tablesNode->first_node("property"); tableNode; tableNode = tableNode->next_sibling("property"))
	{
		ClassTable classTable;

		if (!ParseClassTable(*tableNode, classTable))
		{
			LOG_ERROR << "load class from file <" << path << "> failed! <" << tableNode->contents() << "> node is invalid!";
			return false;
		}

		oneClass.vecTable.push_back(classTable);
	}

	return true;
}

bool ClassModule::ParseClassProperty(rapidxml::xml_node<char> &propertyNode, ClassProperty& classProperty)
{
	rapidxml::xml_attribute<char>* nameAttr = propertyNode.first_attribute("name");
	rapidxml::xml_attribute<char>* typeAttr = propertyNode.first_attribute("type");
	rapidxml::xml_attribute<char>* attrAttr = propertyNode.first_attribute("attr");
	rapidxml::xml_attribute<char> *commentAttr = propertyNode.first_attribute("comment");

	// name����
	if (!nameAttr)
	{
		LOG_ERROR << "parse <" << propertyNode.contents() << "> failed! not found <name> attribute\n";
		return false;
	}

	// type����
	if (!typeAttr)
	{
		LOG_ERROR << "parse <" << propertyNode.contents() << "> failed! not found <type> attribute\n";
		return false;
	}

	classProperty.propertyName = nameAttr->value();
	classProperty.propertyType = ParseVariableType(typeAttr->value());

	// ��飺type���Ա�����Ч
	if (classProperty.propertyType == VariableType_None)
	{
		LOG_ERROR << "parse <" << propertyNode.contents() << "> failed! not support type=\"" << typeAttr->value() << "\"\n";
		return false;
	}

	// attr����
	if (attrAttr)
	{
		std::string attr(attrAttr->value());
	}

	// comment����
	if (commentAttr)
	{
		classProperty.comment = commentAttr->value();
	}
	
	return true;
}

bool ClassModule::ParseClassTable(rapidxml::xml_node<char> &tableNode, ClassTable& classTable)
{
	rapidxml::xml_attribute<>* tableNameAttr = tableNode.first_attribute("name");
	rapidxml::xml_attribute<>* tableAttrAttr = tableNode.first_attribute("attr");
	rapidxml::xml_attribute<> *tableCommentAttr = tableNode.first_attribute("comment");

	// name����
	if (nullptr == tableNameAttr)
	{
		LOG_ERROR << "parse <" << tableNode.contents() << "> failed! not found <name> attribute\n";
		return false;
	}

	classTable.tableName = tableNameAttr->value();

	// attr����
	if (tableAttrAttr)
	{
		std::string attr(tableAttrAttr->value());
	}

	// comment����
	if (tableCommentAttr)
	{
		classTable.comment = tableCommentAttr->value();
	}

	// ������table�ڵ� -> col�ڵ��б�
	for (rapidxml::xml_node<>* colNode = tableNode.first_node("col"); colNode; colNode = colNode->next_sibling("col"))
	{
		ClassTableColumn col;

		rapidxml::xml_attribute<>* colNameAttr = colNode->first_attribute("name");
		rapidxml::xml_attribute<>* colTypeAttr = colNode->first_attribute("type");
		rapidxml::xml_attribute<>* colCommentAttr = colNode->first_attribute("comment");

		// col�ڵ� -> name����
		if (nullptr == colNameAttr)
		{
			LOG_ERROR << "parse <" << colNode->contents() << "> failed! not found \"name\" attribute\n";
			return false;
		}

		// col�ڵ� -> type����
		if (nullptr == colTypeAttr)
		{
			LOG_ERROR << "parse <" << colNode->contents() << "> failed! not found \"type\" attribute\n";
			return false;
		}

		col.propertyType = ParseVariableType(colTypeAttr->value());

		// ��飺type���Ա�����Ч
		if (col.propertyType == VariableType_None)
		{
			LOG_ERROR << "parse <" << colNode->contents() << "> failed! not support type=\"" << colTypeAttr->value() << "\"\n";
			return false;
		}

		// comment����
		if (colCommentAttr)
		{
			col.comment = colCommentAttr->value();
		}

		classTable.vecColumns.push_back(col);
	}

	return true;
}
