/**
 **	File:	gclassjava.cpp
 **
 **	Published / author: 2006-03-14 / grymse@alhem.net
 **/

/*
Copyright (C) 2004  Anders Hedstrom

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#ifndef _WIN32
#include <unistd.h>
#include <stdint.h>
#else
typedef unsigned __int64 uint64_t;
#endif

#include "sql2class.h"
#include "utils.h"

#include "gclassjava.h"


void generate_libjava(FILE *fil,TBLS *table)
{
	char *ptrtotable;
	char classname[200];
	char slask[200];
	char lwrclassname[200];
	char lwrtablename[200];

	strcpy(classname,table -> name.c_str());
	if (mangle_names)
		strlwr(classname);
	strcpy(lwrclassname,classname);
	if (mangle_names)
	{
		if (*classname >= 'a' && *classname <= 'z')
		{
			*classname -= 32;
		}
	}

	strcpy(lwrtablename,table -> name.c_str());
	if (mangle_names)
	{
		strlwr(lwrtablename);
		if (*lwrtablename >= 'a' && *lwrtablename <= 'z')
		{
			*lwrtablename -= 32;
		}
	}

	// -------------------------------------------------------------------
	fprintf(fil, "\n");
	fprintf(fil, "/**\n **  Begin class '%s'\n **/\n",classname);
	fprintf(fil, "\n");

	fprintf(fil, "public class %s\n", classname);
	fprintf(fil, "{\n");

	fprintf(fil, "\tprivate String database;\n");
	fprintf(fil, "\tprivate boolean new_object;\n");
	fprintf(fil, "\tprivate Query handle = new Query();\n");
	fprintf(fil, "\n");

	// variables
	fprintf(fil, "\t// table columns\n");
	for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
	{
		FIELDS *t = *it;
		// long, double, char, text, mediumtext
		if (*t -> ctype)
		{
			std::string intyp;
			std::string suffix;
			std::string type = t -> GetCType(intyp, suffix, make_java);
			fprintf(fil, "\tpublic %s %s%s; // %s",type.c_str(),t -> column,suffix.c_str(),t -> typ);
		}
		else
		{
			fprintf(fil, "\t// %d - %s %s",t -> num,t -> column,t -> typ);
		}
 		if (*t -> comment)
 		{
			fprintf(fil, " //!< %s ",t -> comment);
		}
		fprintf(fil, "\n");

		// Comment from dbdesigner generates a class ptr as member variable
		if (*t -> comment && (ptrtotable = strstr(t -> comment,"ptrtotable: ")) != NULL)
		{
			strcpy(slask,ptrtotable + 12);	// ptrtoclass
			strlwr(slask);
			if (*slask >= 'a' && *slask <= 'z')
			{
				*slask -= 32;
			}
			fprintf(fil, "\tclass %s *_%s;\n",slask,t -> column);
		}
	}
	fprintf(fil, "\n");



	// empty constructor + database
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tpublic %s(String database)",lwrtablename);
	fprintf(fil, "\n\t{\n");
	fprintf(fil, "\t\tthis.database = database;\n");
	fprintf(fil, "\t\tnew_object = true;\n");
	fprintf(fil, "\t\tclear();\n");
	fprintf(fil, "\t}\n\n");


	// data fetch constructor + database
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tpublic %s(String database,String sql) throws SQLException",lwrtablename);
	fprintf(fil, "\n\t{\n");
	fprintf(fil, "\t\tthis.database = database;\n");
	fprintf(fil, "\t\tnew_object = true;\n");
	fprintf(fil, "\t\tspawn(sql);\n");
	fprintf(fil, "\t}\n\n");


	// data fetch constructor from query
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tpublic %s(String database,Query qd,int offset) throws SQLException",lwrtablename);
	fprintf(fil, "\n\t{\n");
	fprintf(fil, "\t\tthis.database = database;\n");
	fprintf(fil, "\t\tnew_object = false;\n");
	fprintf(fil, "\t\tspawn(qd, offset);\n");
	fprintf(fil, "\t}\n\n");


	// constructors from primary index
	{
	for (index_v::iterator it = table -> index.begin(); it != table -> index.end(); it++)
	{
		INDEX *p = *it;
		if (p -> primary || p -> unique)
		{
			fprintf(fil, "\t// -----------------------------------------------------------------------\n");
			fprintf(fil, "\tpublic %s(String database",classname);
			{
			for (fields_v::iterator it = p -> fields.begin(); it != p -> fields.end(); it++)
			{
				FIELDS *f = *it;
				std::string intyp;
				std::string suffix;
				std::string typ = f -> GetCType(intyp, suffix, make_java);
				// long, double, char, text, mediumtext
				if (*f -> ctype)
				{
					fprintf(fil, ",%s i_%s",intyp.c_str(),f -> column);
				}
			}
			}
			fprintf(fil, ") throws SQLException");
			fprintf(fil, "\n\t{\n");
			fprintf(fil, "\t\tthis.database = database;\n");
			fprintf(fil, "\t\tnew_object = true;\n");
			// build sql statement
			fprintf(fil, "\t\tString sql = \"select * from %s where \";\n", table -> name.c_str());
			*slask = 0;
			{
			for (fields_v::iterator it = p -> fields.begin(); it != p -> fields.end(); it++)
			{
				FIELDS *f = *it;
				// long, double, char, text, mediumtext
				if (!strcmp(f -> ctype,"double"))
				{
					if (*slask)
						fprintf(fil, "\t\tsql += \" and \";\n");
					fprintf(fil, "\t\tsql += \"%s='\" + i_%s + \"'\";\n", f -> column, f -> column);
					strcpy(slask," and ");
				}
				else
				if (!strcmp(f -> ctype,"long"))
				{
					if (*slask)
						fprintf(fil, "\t\tsql += \" and \";\n");
					fprintf(fil, "\t\tsql += \"%s='\" + i_%s + \"'\";\n", f -> column, f -> column);
					strcpy(slask," and ");
				}
				else
				if (*f -> ctype)
				{
					if (*slask)
						fprintf(fil, "\t\tsql += \" and \";\n");
					fprintf(fil, "\t\tsql += \"%s='\" + handle.safestr(i_%s) + \"'\";\n", f -> column, f -> column);
					strcpy(slask," and ");
				}
			}
			}
			fprintf(fil, "\t\tspawn(sql);\n");
			//
			fprintf(fil, "\t}\n\n");
		}
	}
	}

	// void Class::select(String ) 'spawn_wrapper'
	//	numera ersatt av Class::Class(db,sql)...
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tpublic void select(String sql) throws SQLException\n\t{\n");
	fprintf(fil, "\t\tspawn(sql);\n");
	fprintf(fil, "\t}\n\n");


	// long Class::insert()
	//	'insert into ...(...) values(...)'
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tpublic long insert() throws SQLException\n\t{\n");

	fprintf(fil, "\t\tString sql = \"insert into %s(",table -> name.c_str());
	*slask = 0;
	{
		for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
		{
			FIELDS *t = *it;
			if (!t -> ignore)
			{
				fprintf(fil, "%s%s",slask,t -> column);
				strcpy(slask,",");
			} // !ignore
		}
	}
	fprintf(fil, ")\";\n");

	strcpy(slask," values(");
	{
		for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
		{
			FIELDS *t = *it;
			if (!t -> ignore)
			{
				// long, double, char, text, mediumtext
				if (!strcmp(t -> ctype,"double"))
				{
					fprintf(fil, "\t\tsql += \"%s\" + this.%s;\n", slask, t -> column);
				}
				else
				if (!strcmp(t -> ctype,"long"))
				{
					fprintf(fil, "\t\tsql += \"%s\" + this.%s;\n", slask, t -> column);
				}
				else
				if (!strcmp(t -> ctype,"text") && !use_stl)
				{
					fprintf(fil, "\t\tsql += \"%s'\" + handle.safestr(this.%s) + \"'\";\n", slask, t -> column);
				}
				else
				if (*t -> ctype)
				{
					fprintf(fil, "\t\tsql += \"%s'\" + handle.safestr(this.%s) + \"'\";\n",slask,t -> column);
				}
				else
				{
					fprintf(fil, "\t\tsql += \"%s''\";\n",slask);
				}
				strcpy(slask,", ");
			} // !ignore
		}
	}
	fprintf(fil, "\t\tsql += \")\";\n");

	fprintf(fil, "\t\ttry\n");
	fprintf(fil, "\t\t{\n");
	{
		fprintf(fil, "\t\t\thandle.dbConnect(database);\n");
		fprintf(fil, "\t\t\thandle.execute(sql);\n");
		fprintf(fil, "\t\t\tnew_object = false;\n");
		fprintf(fil, "\t\t\t// TODO: unsigned long long int inserted_id = q.insert_id();\n");
		{
			fields_l::iterator it = table -> fieldsbase.begin();
			FIELDS *p = *it; //table -> fieldsbase; //table -> primary -> fields[0];
			// long, double, char, text, mediumtext
			if (p && !strcmp(p -> ctype,"long"))
			{
				// %! take care - if first column is an integer type, it will be
				//		set to insert_id()
				fprintf(fil, "\t\t\t// TODO: %s = inserted_id;\n",p -> column);
			}
		}
		fprintf(fil, "\t\t\treturn 0; // TODO: inserted_id;\n");
	}
	fprintf(fil, "\t\t}\n");
//	fprintf(fil, "\t\tcatch (SQLException e)\n");
//	fprintf(fil, "\t\t{\n");
//	fprintf(fil, "\t\t}\n");
	fprintf(fil, "\t\tfinally\n");
	fprintf(fil, "\t\t{\n");
	fprintf(fil, "\t\t\thandle.dbDisconnect();\n");
	fprintf(fil, "\t\t}\n");
//	fprintf(fil, "\t\treturn 0;\n");

	fprintf(fil, "\t}\n\n");


	// void Class::update()
	if (table -> primary)
	{
		fprintf(fil, "\t// -----------------------------------------------------------------------\n");
		fprintf(fil, "\tpublic void update() throws SQLException\n\t{\n");
		fprintf(fil, "\t\tupdate("); //%s);\n",table -> fieldsbase -> column);
		*slask = 0;
		for (fields_v::iterator it = table -> primary -> fields.begin(); it != table -> primary -> fields.end(); it++)
		{
			FIELDS *f = *it;
			fprintf(fil, "%sthis.%s",slask,f -> column);
			strcpy(slask,",");
		}
		fprintf(fil, ");\n");
		fprintf(fil, "\t}\n\n");
	} // if (table -> primary)


	// void Class::update(long num)
	//	'update ... set ... where ...'
	if (table -> primary)
	{
		fprintf(fil, "\t// -----------------------------------------------------------------------\n");
		fprintf(fil, "\tpublic void update(");
		*slask = 0;
		for (fields_v::iterator it = table -> primary -> fields.begin(); it != table -> primary -> fields.end(); it++)
		{
			FIELDS *f = *it;
			std::string intyp;
			std::string suffix;
			std::string typ = f -> GetCType(intyp, suffix, make_java);
			// long, double, char, text, mediumtext
			fprintf(fil, "%s%s i_%s",slask,intyp.c_str(),f -> column);
			strcpy(slask,",");
		}
		fprintf(fil, ") throws SQLException\n\t{\n");
		fprintf(fil, "\t\tString sql = \"\";\n");

		sprintf(slask,"update %s set ",table -> name.c_str());
		{
			for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
			{
				FIELDS *t = *it;
				if (!t -> ignore)
				{
					// long, double, char, text, mediumtext
					if (!strcmp(t -> ctype,"double"))
					{
						fprintf(fil, "\t\tsql += \"%s%s='\" + this.%s + \"'\";\n", slask, t -> column, t -> column);
						strcpy(slask,", ");
					}
					else
					if (!strcmp(t -> ctype,"long"))
					{
						fprintf(fil, "\t\tsql += \"%s%s='\" + this.%s + \"'\";\n", slask, t -> column, t -> column);
						strcpy(slask,", ");
					}
					else
					if (*t -> ctype)
					{
						fprintf(fil, "\t\tsql += \"%s%s='\" + handle.safestr(this.%s) + \"'\";\n", slask, t -> column, t -> column);
						strcpy(slask,", ");
					}
				} // !ignore
			}
		}
		strcpy(slask," where");
		{
			for (fields_v::iterator it = table -> primary -> fields.begin(); it != table -> primary -> fields.end(); it++)
			{
				FIELDS *p = *it;
				// long, double, char, text, mediumtext
				if (!strcmp(p -> ctype,"double"))
				{
					fprintf(fil, "\t\tsql += \"%s %s='\" + i_%s + \"'\";\n", slask, p -> column, p -> column);
				}
				else
				if (!strcmp(p -> ctype,"long"))
				{
					fprintf(fil, "\t\tsql += \"%s %s='\" + i_%s + \"'\";\n", slask, p -> column, p -> column);
				}
				else
				if (*p -> ctype)
				{
					fprintf(fil, "\t\tsql += \"%s %s='\" + handle.safestr(i_%s) + \"'\";\n", slask, p -> column, p -> column);
				}
				//
				strcpy(slask," and");
			}
		}

		fprintf(fil, "\t\ttry\n");
		fprintf(fil, "\t\t{\n");
		{
			fprintf(fil, "\t\t\thandle.dbConnect(database);\n");
			fprintf(fil, "\t\t\thandle.execute(sql);\n");
		}
		fprintf(fil, "\t\t}\n");
//		fprintf(fil, "\t\tcatch (SQLException e)\n");
//		fprintf(fil, "\t\t{\n");
//		fprintf(fil, "\t\t}\n");
		fprintf(fil, "\t\tfinally\n");
		fprintf(fil, "\t\t{\n");
		fprintf(fil, "\t\t\thandle.dbDisconnect();\n");
		fprintf(fil, "\t\t}\n");

		fprintf(fil, "\t}\n\n");
	} // if (table -> primary)


	// void Class::save()
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tpublic void save() throws SQLException\n\t{\n");
	fprintf(fil, "\t\tif (new_object)\n");
	fprintf(fil, "\t\t\tinsert();\n");
	if (table -> primary) // update / erase only available on tables with primary key
	{
		fprintf(fil, "\t\telse\n");
		fprintf(fil, "\t\t\tupdate();\n");
	}
	fprintf(fil, "\t}\n\n");


	// void Class::erase()
	if (table -> primary)
	{
		fprintf(fil, "\t// -----------------------------------------------------------------------\n");
		fprintf(fil, "\tpublic void erase() throws SQLException\n\t{\n");
		fprintf(fil, "\t\tif (!new_object)\n");
		fprintf(fil, "\t\t{\n");
		fprintf(fil, "\t\t\tString sql = \"delete from %s where\";\n",table -> name.c_str());
		*slask = 0;
		for (fields_v::iterator it = table -> primary -> fields.begin(); it != table -> primary -> fields.end(); it++)
		{
			FIELDS *f = *it;
			// long, double, char, text, mediumtext
			if (!strcmp(f -> ctype,"double"))
			{
				fprintf(fil, "\t\t\tsql += \"%s %s='\" + this.%s + \"'\";\n", slask, f -> column, f -> column);
			}
			else
			if (!strcmp(f -> ctype,"long"))
			{
				fprintf(fil, "\t\t\tsql += \"%s %s='\" + this.%s + \"'\";\n", slask, f -> column, f -> column);
			}
			else
			{
				fprintf(fil, "\t\t\tsql += \"%s %s='\" + handle.safestr(this.%s) + \"'\";\n",slask,f -> column,f -> column);
			}
			strcpy(slask," and");
		}
		//
		fprintf(fil, "\t\t\ttry\n");
		fprintf(fil, "\t\t\t{\n");
		{
			fprintf(fil, "\t\t\t\thandle.dbConnect(database);\n");
			fprintf(fil, "\t\t\t\thandle.execute(sql);\n");
		}
		fprintf(fil, "\t\t\t}\n");
//		fprintf(fil, "\t\t\tcatch (SQLException e)\n");
//		fprintf(fil, "\t\t\t{\n");
//		fprintf(fil, "\t\t\t}\n");
		fprintf(fil, "\t\t\tfinally\n");
		fprintf(fil, "\t\t\t{\n");
		fprintf(fil, "\t\t\t\thandle.dbDisconnect();\n");
		fprintf(fil, "\t\t\t}\n");

		fprintf(fil, "\t\t}\n");
		fprintf(fil, "\t}\n\n");
	} // if (table -> primary)

/*
	// void Class::xml()
	fprintf(fil, "\tstd::string xml()\n\t{\n");
	fprintf(fil, "\t\tQuery q(*database);\n");
	fprintf(fil, "\t\tstd::string dest;\n");
	{
	bool need_slask = false;
	for (t = table -> fieldsbase; t && !need_slask; t = t -> next)
		if (!strcmp(t -> ctype,"double") || !strcmp(t -> ctype,"long"))
			need_slask = true;
	if (need_slask)
		fprintf(fil, "\t\tchar slask[200];\n");
	}

	strcpy(slask,classname);
	strupr(slask);
	fprintf(fil, "\t\tdest = \"<%s>\";\n",slask);

	for (t = table -> fieldsbase; t; t = t -> next)
	{
		strcpy(slask,t -> column);
		strupr(slask);
		// long, double, char, text, mediumtext
		if (t -> enum_set)
		{
			fprintf(fil, "\t\tdest += \"<%s>\" + q.xmlsafestr(this -> %s.String()) + \"</%s>\";\n",slask,t -> column,slask);
		}
		else
		if (!strcmp(t -> ctype,"double"))
		{
			fprintf(fil, "\t\tsprintf(slask,\"<%s>%%f</%s>\",this -> %s);\n",slask,slask,t -> column);
			fprintf(fil, "\t\tdest += slask;\n");
		}
		else
		if (!strcmp(t -> ctype,"long"))
		{
			std::string l = (t -> bitsize >= 32) ? "l" : "";
			char c = t -> uns ? 'u' : 'd';
			// %! bigint
			if (t -> bitsize == 64)
				l = "ll";
			fprintf(fil, "\t\tsprintf(slask,\"<%s>%%%s%c</%s>\",this -> %s);\n",slask,l.c_str(),c,slask,t -> column);
			fprintf(fil, "\t\tdest += slask;\n");
		}
		else
		if (!strcmp(t -> ctype,"text") && !use_stl)
		{
			fprintf(fil, "\t\tdest += \"<%s>\" + (this -> %s ? q.xmlsafestr(this -> %s) : \"\") + \"</%s>\";\n",slask,t -> column,t -> column,slask);
		}
		else
		if (!strcmp(t -> ctype,"text"))
		{
			fprintf(fil, "\t\tdest += \"<%s>\" + q.xmlsafestr(this -> %s) + \"</%s>\";\n",slask,t -> column,slask);
		}
		else
		if (!strcmp(t -> ctype,"mediumtext"))
		{
			// vector<string>
		}
		else
		if (*t -> ctype)
		{
			fprintf(fil, "\t\tdest += \"<%s>\" + q.xmlsafestr(this -> %s) + \"</%s>\";\n",slask,t -> column,slask);
		}
	}
	strcpy(slask,classname);
	strupr(slask);
	fprintf(fil, "\t\tdest += \"</%s>\";\n",slask);

	fprintf(fil, "\t\treturn dest;\n");
	fprintf(fil, "\t}\n\n");


	// void Class::xml()
	fprintf(fil, "\tstd::string xml(String tag,String xvalx)\n\t{\n");
	fprintf(fil, "\t\tQuery q(*database);\n");
	fprintf(fil, "\t\tstd::string dest;\n");
	{
	bool need_slask = false;
//	for (t = table -> fieldsbase; t && !need_slask; t = t -> next)
	for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
	{
		FIELDS *t = *it;
		if (!strcmp(t -> ctype,"double") || !strcmp(t -> ctype,"long"))
		{
			need_slask = true;
			break;
		}
	}
	if (need_slask)
		fprintf(fil, "\t\tchar slask[200];\n");
	}

	strcpy(slask,classname);
	strupr(slask);
	fprintf(fil, "\t\tdest = \"<%s \" + tag + \"=\\\"\" + xvalx + \"\\\">\";\n",slask);

//	for (t = table -> fieldsbase; t; t = t -> next)
	for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
	{
		FIELDS *t = *it;
		strcpy(slask,t -> column);
		strupr(slask);
		// long, double, char, text, mediumtext
		if (t -> enum_set)
		{
			fprintf(fil, "\t\tdest += \"<%s>\" + q.xmlsafestr(this -> %s.String()) + \"</%s>\";\n",slask,t -> column,slask);
		}
		else
		if (!strcmp(t -> ctype,"double"))
		{
			fprintf(fil, "\t\tsprintf(slask,\"<%s>%%f</%s>\",this -> %s);\n",slask,slask,t -> column);
			fprintf(fil, "\t\tdest += slask;\n");
		}
		else
		if (!strcmp(t -> ctype,"long"))
		{
			std::string l = (t -> bitsize >= 32) ? "l" : "";
			char c = t -> uns ? 'u' : 'd';
			// %! bigint
			if (t -> bitsize == 64)
				l = "ll";
			fprintf(fil, "\t\tsprintf(slask,\"<%s>%%%s%c</%s>\",this -> %s);\n",slask,l.c_str(),c,slask,t -> column);
			fprintf(fil, "\t\tdest += slask;\n");
		}
		else
		if (!strcmp(t -> ctype,"text") && !use_stl)
		{
			fprintf(fil, "\t\tdest += \"<%s>\" + (this -> %s ? q.xmlsafestr(this -> %s) : \"\") + \"</%s>\";\n",slask,t -> column,t -> column,slask);
		}
		else
		if (!strcmp(t -> ctype,"text"))
		{
			fprintf(fil, "\t\tdest += \"<%s>\" + q.xmlsafestr(this -> %s) + \"</%s>\";\n",slask,t -> column,slask);
		}
		else
		if (!strcmp(t -> ctype,"mediumtext"))
		{
			// vector<string>
		}
		else
		if (*t -> ctype)
		{
			fprintf(fil, "\t\tdest += \"<%s>\" + q.xmlsafestr(this -> %s) + \"</%s>\";\n",slask,t -> column,slask);
		}
	}
	strcpy(slask,classname);
	strupr(slask);
	fprintf(fil, "\t\tdest += \"</%s>\";\n",slask);

	fprintf(fil, "\t\treturn dest;\n");
	fprintf(fil, "\t}\n\n");
*/

	// int Class::num_cols()
	int num_cols = table -> fieldsbase.size();
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tpublic int num_cols()\n");
	fprintf(fil, "\t{\n");
	fprintf(fil, "\t\treturn %d;\n",num_cols); //listlen(table -> fieldsbase));
	fprintf(fil, "\t}\n\n");


// private

	// clear fields
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tprivate void clear()\n\t{\n");
	{
		for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
		{
			FIELDS *t = *it;
			if (t -> set)
			{
				uint64_t val = 1;
				for (std::vector<std::string>::iterator it = t -> mvec.begin(); it != t -> mvec.end(); it++)
				{
					std::string txt = *it;
					fprintf(fil, "\t\tmmap_%s[\"%s\"] = %lld;\n",t -> column,txt.c_str(),val);
					val = val << 1;
				}
			}
			else
			if (t -> enum_set) // enum
			{
				uint64_t val = 1;
				for (std::vector<std::string>::iterator it = t -> mvec.begin(); it != t -> mvec.end(); it++)
				{
					std::string txt = *it;
					fprintf(fil, "\t\tmmap_%s[\"%s\"] = %lld;\n",t -> column,txt.c_str(),val);
					val++;
				}
			}
		}
	}
	{
		for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
		{
			FIELDS *t = *it;
			// long, double, char, text, mediumtext
			if (!strcmp(t -> ctype,"double"))
			{
				fprintf(fil, "\t\t%s = 0;\n",t -> column);
			}
			else
			if (!strcmp(t -> ctype,"long"))
			{
				fprintf(fil, "\t\t%s = 0;\n",t -> column);
			}
			else
			if (!strcmp(t -> ctype,"text") && !use_stl)
			{
				fprintf(fil, "\t\t%s = NULL;\n",t -> column);
			}
			else
			if (!strcmp(t -> ctype,"mediumtext"))
			{
				// a vector<string> is created empty
			}
			else
			if (*t -> ctype && !use_stl)
			{
				fprintf(fil, "\t\t*%s = 0;\n",t -> column);
				if (t -> length == 1) // char
				{
					fprintf(fil, "\t\t%s[1] = 0;\n",t -> column);
				}
			}
			else
			{
				fprintf(fil, "\t\t// %d - %s %s\n",t -> num,t -> column,t -> typ);
			}

			// more dbdesigner comment special
			if (*t -> comment && (ptrtotable = strstr(t -> comment,"ptrtotable: ")) != NULL)
			{
				strcpy(slask,ptrtotable + 12);	// ptrtoclass
				strlwr(slask);
				fprintf(fil, "\t\t_%s = NULL;\n",t -> column);
			}
		}
	}
	fprintf(fil, "\t}\n\n");


	// spawn from query
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tprivate void spawn(String sql) throws SQLException\n\t{\n");
	fprintf(fil, "\t\tclear();\n");
	fprintf(fil, "\n");
	fprintf(fil, "\t\ttry\n");
	fprintf(fil, "\t\t{\n");
	{
		fprintf(fil, "\t\t\thandle.dbConnect(database);\n");

		fprintf(fil, "\t\t\thandle.get_result(sql);\n");
		fprintf(fil, "\t\t\tif (handle.fetch_row())\n");
		fprintf(fil, "\t\t\t{\n");
		for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
		{
			FIELDS *t = *it;
			int i = 0;
			// long, double, char, text, mediumtext
			if (!strcmp(t -> ctype,"double"))
			{
				fprintf(fil, "\t\t\t\tthis.%s = handle.dbGetDouble(%d);",t -> column,t -> num + 1);
				sprintf(slask,"%d",t -> num);
				i = strlen(t -> column) + strlen(slask);
			}
			else
			if (!strcmp(t -> ctype,"long"))
			{
				if (t -> uns)
				{
					fprintf(fil, "\t\t\t\tthis.%s = handle.getuval(%d);",t -> column,t -> num + 1);
					sprintf(slask,"%d",t -> num);
					i = strlen(t -> column) + strlen(slask) + 1;
				}
				else
				{
					fprintf(fil, "\t\t\t\tthis.%s = handle.dbGetLong(%d);",t -> column,t -> num + 1);
					sprintf(slask,"%d",t -> num);
					i = strlen(t -> column) + strlen(slask);
				}
			}
			else
			if (!strcmp(t -> ctype,"text"))
			{
				fprintf(fil, "\t\t\t\tthis.%s = handle.dbGetString(%d);",t -> column,t -> num + 1);
				sprintf(slask,"%d",t -> num);
				i = strlen(t -> column) + strlen(slask);
			}
			else
			if (*t -> ctype)
			{
				fprintf(fil, "\t\t\t\tthis.%s = handle.dbGetString(%d);",t -> column,t -> num + 1);
				sprintf(slask,"%d",t -> num);
				i = strlen(t -> column) + strlen(slask);
			}
			else
			{
				fprintf(fil, "\t\t\t\t// %d - %s %s\n",t -> num,t -> column,t -> typ);
				i = 0;
			}
			if (i)
			{
				strcpy(slask,"																				");
				slask[40 - i] = 0;
				fprintf(fil, "%s// %d - %s %s\n",slask,t -> num,t -> column,t -> typ);
			}
		}
		fprintf(fil, "\t\t\t\tnew_object = false;\n");
		fprintf(fil, "\t\t\t}\n");
		fprintf(fil, "\t\t\thandle.free_result();\n");
	}
	fprintf(fil, "\t\t}\n");
//	fprintf(fil, "\t\tcatch (SQLException e)\n");
//	fprintf(fil, "\t\t{\n");
//	fprintf(fil, "\t\t}\n");
	fprintf(fil, "\t\tfinally\n");
	fprintf(fil, "\t\t{\n");
	fprintf(fil, "\t\t\thandle.dbDisconnect();\n");
	fprintf(fil, "\t\t}\n");

	fprintf(fil, "\t}\n\n");


	// spawn from query
	fprintf(fil, "\t// -----------------------------------------------------------------------\n");
	fprintf(fil, "\tprivate void spawn(Query qd,int offset) throws SQLException\n");
	fprintf(fil, "\t{\n");
	fprintf(fil, "\t\tclear();\n");
	fprintf(fil, "\n");
	{
		for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
		{
			FIELDS *t = *it;
			int i = 0;
			// long, double, char, text, mediumtext
			if (!strcmp(t -> ctype,"double"))
			{
				fprintf(fil, "\t\tthis.%s = qd.dbGetDouble(%d + offset);",t -> column,t -> num + 1);
				sprintf(slask,"%d",t -> num);
				i = strlen(t -> column) + strlen(slask);
			}
			else
			if (!strcmp(t -> ctype,"long"))
			{
				if (t -> uns)
				{
					fprintf(fil, "\t\tthis.%s = qd -> getuval(%d + offset);",t -> column,t -> num + 1);
					sprintf(slask,"%d",t -> num);
					i = strlen(t -> column) + strlen(slask) + 1;
				}
				else
				{
					fprintf(fil, "\t\tthis.%s = qd.dbGetLong(%d + offset);",t -> column,t -> num + 1);
					sprintf(slask,"%d",t -> num);
					i = strlen(t -> column) + strlen(slask);
				}
			}
			else
			if (!strcmp(t -> ctype,"text"))
			{
				fprintf(fil, "\t\tthis.%s = qd.dbGetString(%d + offset);",t -> column,t -> num + 1);
				sprintf(slask,"%d",t -> num);
				i = strlen(t -> column) + strlen(slask);
			}
			else
			if (*t -> ctype)
			{
				fprintf(fil, "\t\tthis.%s = qd.dbGetString(%d + offset);",t -> column,t -> num + 1);
				sprintf(slask,"%d",t -> num);
				i = strlen(t -> column) + strlen(slask);
			}
			else
			{
				fprintf(fil, "\t\t// %d - %s %s\n",t -> num,t -> column,t -> typ);
				i = 0;
			}
			if (i)
			{
				strcpy(slask,"																				");
				slask[40 - i] = 0;
				fprintf(fil, "%s// %d - %s %s\n",slask,t -> num,t -> column,t -> typ);
			}
		}
	}
	fprintf(fil, "\t}\n\n");


	// -------------------------------------------------------------------
	fprintf(fil, "} // End of implementation of class '%s'\n",classname);
	fprintf(fil, "\n");

} // generate_libjava


