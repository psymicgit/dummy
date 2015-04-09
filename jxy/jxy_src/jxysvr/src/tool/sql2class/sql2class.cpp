/**
 **	File:	sql2class.cpp
 **
 **	Published / author: 2004-04-14 / grymse@alhem.net
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
#define strcasecmp stricmp
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#else
#define strncasecmp strnicmp
#endif

#include "sql2class.h"
#include "utils.h"
#include "gclass.h"
#include "gclasslib.h"
#include "Parse.h"


tbls_l tblsbase;
std::string namespc;
std::string baseclass;
short container = 0;
bool mediumtext = false;
bool use_stl = true;
bool make_java = false;
std::string package = "db";
bool license = true;
std::string queryclass = "Query";
std::string cmdline;
bool make_getset = true;
bool mangle_names = true;
// wrapper selection
bool use_sqlite = false;
bool use_odbc = false;
bool use_wrapped = false;
//
static	char the_str[30000];
static	int the_ptr;

#define C the_str[the_ptr]
#define C1 the_str[the_ptr + 1]


void get_next_token(char *s)
{
    while (C == ' ' || C == 9)
        the_ptr++;
    if (!C)
    {
        *s = 0;
        return;
    }
    int x = the_ptr;
    if (isalpha(C) || C == '_')
    {
        while (isalnum(C) || C == '_')
        {
            the_ptr++;
        }
        strncpy(s, the_str + x, the_ptr - x);
        s[the_ptr - x] = 0;
        return;
    }
    if (isdigit(C))
    {
        while (isdigit(C))
            the_ptr++;
        strncpy(s, the_str + x, the_ptr - x);
        s[the_ptr - x] = 0;
        return;
    }
    if ((C == '/' && C1 == '*') ||
            (C == '*' && C1 == '/')
       )
    {
        *s = C;
        s[1] = C1;
        s[2] = 0;
        the_ptr += 2;
        return;
    }
    if (C == '"')
    {
        the_ptr++;
        while (C && C != '"')
        {
            the_ptr++;
        }
        if (C == '"')
        {
            the_ptr++;
        }
        strncpy(s, the_str + x, the_ptr - x);
        s[the_ptr - x] = 0;
        return;
    }
    if (C == '\'')
    {
        the_ptr++;
        while (C && C != '\'')
        {
            the_ptr++;
        }
        if (C == '\'')
        {
            the_ptr++;
        }
        strncpy(s, the_str + x, the_ptr - x);
        s[the_ptr - x] = 0;
        return;
    }
    if (C == '`')
    {
        the_ptr++;
        while (C && C != '`')
        {
            the_ptr++;
        }
        if (C == '`')
        {
            the_ptr++;
        }
        strncpy(s, the_str + x, the_ptr - x);
        s[the_ptr - x] = 0;
        return;
    }
    *s = C;
    s[1] = 0;
    the_ptr++;
}


std::string FIELDS::GetCType(std::string& intyp, std::string& suffix, bool java)
{
    std::string strTyp;

    suffix = "";
    // long, double, char, text, mediumtext
    if (set)
    {
        strTyp = "set_t";
        intyp = "const std::string& ";
    }
    else if (enum_set)
    {
        strTyp = "enum_t";
        intyp = "const std::string& ";
    }
    else if (!strcmp(ctype, "long"))
    {
        if (java)
        {
            intyp = ctype;
            return "long";
        }
        switch (bitsize)
        {
        case 8:
            strTyp = uns ? "unsigned char" : "char";
            break;
        case 16:
            strTyp = uns ? "unsigned short" : "short";
            break;
        case 24:
        case 32:
            strTyp = uns ? "unsigned long" : "long";
            break;
        case 64:
            strTyp = uns ? "uint64_t" : "int64_t";
            break;
        }
        intyp = strTyp;
    }
    else if (!strcmp(ctype, "double"))
    {
        strTyp = "double";
        intyp = strTyp;
    }
    else if (!strncmp(ctype, "char", 4))
    {
        if (java)
        {
            intyp = "String";
            return "String";
        }
        if (use_stl)
        {
            strTyp = "std::string";
        }
        else
        {
            char slask[100];
            sprintf(slask, "[%d]", length + 1);
            strTyp = "char";
            suffix = slask;
        }
        intyp = "const std::string& ";
    }
    else if (!strcmp(ctype, "text") || !strcmp(ctype, "longvarchar"))
    {
        if (java)
        {
            intyp = "String";
            return "String";
        }
        strTyp = use_stl ? "std::string" : "char *";
        intyp = "const std::string& ";
    }
    else if (!strcmp(ctype, "mediumtext"))
    {
        if (java)
        {
            intyp = "String";
            return "String";
        }
        strTyp = "std::vector<std::string>";
        intyp = "std::vector<std::string>& ";
    }
    else
    {
        printf("Warning. Unsupported type '%s'\n", typ);
        intyp = "";
    }
    return strTyp;
}


bool check_index(TBLS *table, INDEX *i)
{
    for (index_v::iterator it = table -> index.begin(); it != table -> index.end(); it++)
    {
        INDEX *p = *it;
        if (p -> primary || p -> unique)
        {
            fields_v::iterator it1 = i -> fields.begin();
            fields_v::iterator it2 = p -> fields.begin();
            while (it1 < i -> fields.end() && it2 < p -> fields.end())
            {
                FIELDS *f1 = *it1;
                FIELDS *f2 = *it2;
                if (!strcmp(f1 -> ctype, "double") || !strcmp(f2 -> ctype, "double"))
                {
                    if (strcmp(f1 -> ctype, f2 -> ctype))
                    {
                        return false;
                    }
                }
                else if (!strcmp(f1 -> ctype, "long") || !strcmp(f2 -> ctype, "long"))
                {
                    if (strcmp(f1 -> ctype, f2 -> ctype))
                    {
                        return false;
                    }
                }
                it1++;
                it2++;
                if (it1 == i -> fields.end() && it2 == p -> fields.end())
                {
                    return true;
                }
                if (it1 == i -> fields.end() || it2 == p -> fields.end())
                {
                    return false;
                }
            }
        }
    }
    return true;
}


bool reserved_name(const std::string& slask)
{
    if (!strcasecmp(slask.c_str(), "typeid") ||
            !strcasecmp(slask.c_str(), "typename") ||
            !strcasecmp(slask.c_str(), "public")
       )
        return true;
    return false;
}


void parse_index(TBLS *table, bool primary, bool unique, char *name, char *fields)
{
    INDEX *p = new INDEX(name, primary, unique);
    char tmp[1000];
    strcpy(tmp, fields + 1);
    tmp[strlen(tmp) - 1] = 0;

    Parse pa(tmp, ",");
    char slask[200];
    pa.getword(slask);
    while (*slask)
    {
        FIELDS *f = NULL;
        Parse pa2(slask, "(");
        pa2.getword(slask);
        if (*slask == '`' || *slask == '"' || *slask == '\'')
        {
            memcpy(slask, slask + 1, strlen(slask) + 1);
            slask[strlen(slask) - 1] = 0;
        }
        if (reserved_name(slask))
        {
            std::string tmp = "_";
            tmp += slask;
            strcpy(slask, tmp.c_str());
        }
        for (fields_l::iterator it = table -> fieldsbase.begin(); it != table -> fieldsbase.end(); it++)
        {
            f = *it;
            if (!strcasecmp(f -> column, slask))
            {
                break;
            }
            f = NULL;
        }
        if (!f)
        {
            printf("Can't resolve column name in index '%s': '%s'\n", name, slask);
            delete p;
            return;
        }
        p -> fields.push_back(f);
        //
        pa.getword(slask);
    }
    p -> ambig = (primary || unique) ? check_index(table, p) : false;
    table -> index.push_back(p);
    if (p -> primary)
    {
        table -> primary = p;
    }
    if (p -> unique)
    {
        table -> unique = p;
    }
}


short inset(const char *s, const char *set)
{
    std::string str;
    char slask[200];
    {
        Parse pa( (char *)s, "()");
        pa.getword(slask);
        str = slask;
    }
    Parse pa( (char *)set);
    short r = 1;

    pa.getword(slask);
    while (*slask)
    {
        if (!strcasecmp(str.c_str(), slask))
        {
            return r;
        }
        pa.getword(slask);
        r++;
    }
    return 0;
}


FIELDS *reg_column(TBLS *table, const char *n, const char *typ, bool uns)
{
    FIELDS *t;
    short l;	// type name length
    char len[200];
    char name[200];

    strcpy(name, n);
    if (*name == '`' || *name == '"' || *name == '\'')
    {
        memcpy(name, name + 1, strlen(name + 1) + 1);
        name[strlen(name) - 1] = 0;
    }
    std::string sqlname = name; // save original name
    if (reserved_name(name))
    {
        std::string tmp = "_";
        tmp += name;
        strcpy(name, tmp.c_str());
    }
    if (mangle_names)
    {
        strlwr(name);
    }

    t = new FIELDS;
    t -> num = table -> fieldsbase.size(); //listlen(table -> fieldsbase);
    strcpy(t -> column, name);
    strcpy(t -> typ, typ);
    *t -> ctype = 0;
    t -> length = 0;
    *t -> comment = 0;
    t -> ignore = false; // wont use field in insert/update if true
    t -> uns = uns;
    t -> bitsize = 0;
    t -> enum_set = false;
    t -> set = false;
    t -> sqlname = sqlname;

    if ((l = inset(typ, "tinyint bit bool boolean smallint mediumint int integer")) != 0)
    {
        strcpy(t -> ctype, "long");
        if (inset(typ, "tinyint bit bool boolean"))
            t -> bitsize = 8;
        else if (inset(typ, "smallint"))
            t -> bitsize = 16;
        else if (inset(typ, "mediumint"))
            t -> bitsize = 24;
        else if (inset(typ, "int integer"))
            t -> bitsize = 32;
    }
    else if (!strncasecmp(typ, "bigint", 6))
    {
        strcpy(t -> ctype, "long");
        t -> bitsize = 64;
    }
    else if (inset(typ, "float double double_precision real decimal dec numeric fixed")) //!strncasecmp(typ,"decimal",7) || !strncasecmp(typ,"float",5))
    {
        strcpy(t -> ctype, "double");
    }
    else if (!strncasecmp(typ, "timestamp", 9))
    {
        strcpy(t -> ctype, "char[40]");
        t -> length = 40;
        t -> ignore = true; // use timestamp for timestamps
    }
    else if (inset(typ, "date datetime time")) //!strcasecmp(typ,"datetime") || !strcasecmp(typ,"date") || !strcasecmp(typ,"time") || !strncasecmp(typ,"timestamp",9) )
    {
        strcpy(t -> ctype, "char[40]");
        t -> length = 40;
    }
    else if (!strncasecmp(typ, "year", 4))
    {
        strcpy(t -> ctype, "long");
        t -> uns = true;
        t -> bitsize = 16;
    }
    else
    {
        // string types
        if (!strncasecmp(typ, "char", 4))
        {
            Parse pa( (char *)typ, "()");
            pa.getword();
            pa.getword(len);
            if (!*len)
                strcpy(len, "1");
            t -> length = atoi(len); // "0" allowed
            sprintf(t -> ctype, "char[%d]", t -> length + 1);
        }
        else if (!strncasecmp(typ, "varchar", 7))
        {
            Parse pa( (char *)typ, "()");
            pa.getword(); // type name
            t -> length = pa.getvalue();
            sprintf(t -> ctype, "char[%d]", t -> length + 1);
        }
        else if (!strcasecmp(typ, "tinytext"))
        {
            t -> length = 255;
            strcpy(t -> ctype, "char[256]");
        }
        else if (!strcasecmp(typ, "text") || !strcasecmp(typ, "longvarchar"))
        {
            strcpy(t -> ctype, "text");
            t -> length = -1;
        }
        else if (!strcasecmp(typ, "mediumtext") || !strcasecmp(typ, "longtext") )
        {
            strcpy(t -> ctype, "mediumtext");
            t -> length = -1;
//		t -> ignore = true;
            mediumtext = true;
        }
		else if (!strcasecmp(typ, "blob"))
		{
			t -> length = 65535;
			strcpy(t -> ctype, "struct");
		}
        else
        {
            /*
            	color enum('Red','Green','Blue') default NULL,
            	colormix set('Red','Green','Blue') default NULL,
            */
            if (!strncasecmp(typ, "enum", 4)) // 65535 values
            {
                /*
                		strcpy(t -> ctype, "long");
                		t -> uns = true;
                		t -> bitsize = 16;
                */
                t -> enum_set = true;
                strcpy(t -> ctype, "char[256]");
                t -> length = 255;
                //
                Parse pa( (char *)typ, "(',)");
                char slask[200];
                pa.getword(); // 'enum'
                pa.getword(slask);
                while (*slask)
                {
                    t -> mvec.push_back( slask );
                    //
                    pa.getword(slask);
                }
            }
            else if (!strncasecmp(typ, "set", 3)) // 64 bits
            {
                /*
                		strcpy(t -> ctype, "long");
                		t -> uns = true;
                		t -> bitsize = 64;
                */
                t -> enum_set = true;
                t -> set = true;
                strcpy(t -> ctype, "char[256]");
                t -> length = 255;
                //
                Parse pa( (char *)typ, "(',)");
                char slask[200];
                pa.getword(); // 'set'
                pa.getword(slask);
                while (*slask)
                {
                    t -> mvec.push_back( slask );
                    //
                    pa.getword(slask);
                }
            }
            else
            {
                printf("Warning. Unsupported column data type: %s.%s '%s'\n", table -> name.c_str(), name, typ);
                *t -> ctype = 0;
                t -> length = 0;
                t -> ignore = true; // seems like a good idea
            }
        }
    }
    table -> fieldsbase.push_back(t);
    return t;
}


/*
 *
 */

int main(int argc, char *argv[])
{
    FILE *fil;
    TBLS *table = NULL;
    FIELDS *current = NULL;
//	Parse *pa;
    int loop;
    short state = 0;
    short makerel = 0;
    short overwrite = 0;
    short global = 1;
    short build = 0;
    bool primary = false;
    bool unique = false;
    bool uns = false;
    bool pri = false; // primary key defined in column definition (sqlite)
    bool auto_increment = false;
    char slask[32000];
    char current_column[200];
    char filename[200];
    char dbname[200];
    char includedir[200];
    char comment[200];
    char libname[200];
    char prefix[200];
    char fields[1000];
    char index_name[100];
    std::string current_type;

    strcpy(prefix, "/usr/devel");
    *filename = 0;
    *libname = 0;
    *dbname = 0;
    namespc = "";
    baseclass = "";

    // _win32 fix
#ifdef _WIN32
    global = 0;
    build = 0;
#endif

    for (loop = 0; loop < argc; loop++)
    {
        if (loop)
        {
            cmdline += " ";
        }
        if (strstr(argv[loop], " "))
        {
            if (!strstr(argv[loop], "\""))
            {
                cmdline += "\"";
                cmdline += argv[loop];
                cmdline += "\"";
            }
            else
            {
                cmdline += "'";
                cmdline += argv[loop];
                cmdline += "'";
            }
        }
        else
        {
            cmdline += argv[loop];
        }
    }
    for (loop = 1; loop < argc; loop++)
    {
        if (!strcmp(argv[loop], "-container"))
            container++;
        else if (!strcmp(argv[loop], "-makerel"))
            makerel++;
        else if (!strcmp(argv[loop], "-overwrite"))
            overwrite++;
        else if (!strcmp(argv[loop], "-prefix") && loop < argc - 1)
        {
            loop++;
            strcpy(prefix, argv[loop]);
            if (prefix[strlen(prefix) - 1] == '/')
                prefix[strlen(prefix) - 1] = 0;
        }
        else if (!strcmp(argv[loop], "-database") && loop < argc - 1)
        {
            loop++;
            strcpy(dbname, argv[loop]);
            strlwr(dbname);
            *dbname -= 32;
        }
        else if (!strcmp(argv[loop], "-global"))
        {
            printf("-global IS the default already\n");
            global++;
        }
        else if (!strcmp(argv[loop], "-global-"))
            global = 0;
        else if (*argv[loop] != '-')
            strcpy(filename, argv[loop]);
        else if (!strcmp(argv[loop], "-lib") && loop < argc - 1)
        {
            loop++;
            strcpy(libname, argv[loop]);
        }
        else if (!strcmp(argv[loop], "-build"))
            build++;
        else if (!strcmp(argv[loop], "-namespace") && loop < argc - 1)
        {
            loop++;
            namespc = argv[loop];
        }
        else if (!strcmp(argv[loop], "-baseclass") && loop < argc - 1)
        {
            loop++;
            baseclass = argv[loop];
        }
        else if (!strcmp(argv[loop], "-stl"))
        {
            printf("-stl IS the default already\n");
            use_stl = true;
        }
        else if (!strcmp(argv[loop], "-stl-"))
            use_stl = false;
        else if (!strcmp(argv[loop], "-sqlite"))
            use_sqlite = true;
        else if (!strcmp(argv[loop], "-odbc"))
            use_odbc = true;
        else if (!strcmp(argv[loop], "-wrapped"))
            use_wrapped = true;
        else if (!strcmp(argv[loop], "-license"))
            license = false;
        else if (!strcmp(argv[loop], "-java"))
            make_java = true;
        else if (!strcmp(argv[loop], "-package") && loop < argc - 1)
        {
            loop++;
            package = argv[loop];
        }
        else if (!strcmp(argv[loop], "-queryclass") && loop < argc - 1)
        {
            loop++;
            queryclass = argv[loop];
        }
        else if (!strcmp(argv[loop], "-getset"))
        {
            make_getset = false;
        }
        else if (!strcmp(argv[loop], "-mangle"))
        {
            mangle_names = false;
        }
        else
        {
            printf("Unknown option '%s'...\n", argv[loop]);
        }
    } // for
    sprintf(includedir, "%s/include", prefix);

    if (argc < 2)
    {
        printf("Usage: %s [options] <.sql file>\n", *argv);
        printf(" select wrapper library:\n");
        printf("  -sqlite       use sqlite3, not mysql\n");
        printf("  -odbc         use odbcwrapped, not mysql\n");
        printf("  -wrapped      use sqlwrapped\n");
        printf(" options:\n");
        printf("  -prefix <path>set global directory (default '/usr/devel')\n");
        printf("  -build	build lib if '-global' and '-lib' is used\n");
        printf("  -global	generate .h files to global include directory (default)\n");
        printf("		 (default /usr/devel/include/)\n");
        printf("  -global-	turn off global generation\n");
        printf("  -stl          use std::string in generated classes (default)\n");
        printf("  -stl-         do not use STL in generated classes\n");
        printf("  -lib <name>	generate class library\n");
        printf("  -namespace <name> use namespace\n");
        printf("  -overwrite	existing files are overwritten\n");
//		printf("  -makerel	make relation class for three input tables\n");
//		printf("		 (tbl1-1, tbl2-n, tbl3-link table)\n");
        printf("  -database <name> enter database name in generated file names\n");
        printf("		 (sql<Databasename>Tbl<table>.h)\n");
        printf("  -baseclass <name> generated classes inherits from...\n");
        printf("  -container	generate old style container class also\n");
        printf("  -license      suppress license in generated code\n");
        printf("  -getset       generate public table variables instead of get/set methods\n");
        printf("  -mangle       do not convert table and column names to lowercase\n");
        printf(" The following java options are still experimental and does not\n");
        printf(" have a *wrapped library supporting the generated code.\n");
        printf("  -java         generate java, not C++\n");
        printf("  -package      java package in generated files\n");
        printf("  -queryclass   query class implementation to use\n");
        exit(-1);
    }
    if (!*filename)
    {
        fil = stdin;
    }
    else
    {
        if ((fil = fopen(filename, "rt")) == NULL)
        {
            printf("%s: Couldn't open '%s'...\n", *argv, argv[1]);
            exit(-1);
        }
    }

// read tables
    bool wait_for = false;
    fgets(the_str, 30000, fil);
    while (!feof(fil))
    {
        while (strlen(the_str) && (the_str[strlen(the_str) - 1] == 13 ||
                                   the_str[strlen(the_str) - 1] == 10))
        {
            the_str[strlen(the_str) - 1] = 0;
        }
        /*
        		pa = new Parse(slask);
        		pa -> setbreak(',');
        		pa -> disablebreak('(');
        		pa -> enablebreak(')');
        		pa -> getword(slask);
        */
        the_ptr = 0; // init_parser...
        get_next_token(slask);
        while (*slask)
        {
            printf("<%s>\n", slask);
            switch (state)
            {
            case 0:		// wait for 'create'
                if (!strcasecmp(slask, "create"))
                    state = 1;
                break;
            case 1:		// wait for 'table'
                if (!strcasecmp(slask, "table"))
                    state = 2;
                else
                {
                    current = NULL;
                    state = 0;
                }
                break;
            case 2:		// get table name
                table = new TBLS;
                if (*slask == '`' || *slask == '"' || *slask == '\'')
                {
                    memcpy(slask, slask + 1, strlen(slask) + 1);
                    slask[strlen(slask) - 1] = 0;
                }
                table -> name = slask;
                while (table -> fieldsbase.size())
                {
                    fields_l::iterator it = table -> fieldsbase.begin();
                    FIELDS *t = *it;
                    delete t;
                    table -> fieldsbase.erase(it);
                }
                table -> primary = NULL;
                table -> unique = NULL;
                tblsbase.push_back(table);

                state = 3;
                break;
            case 3:		// get '('
                if (!strcmp(slask, "("))
                    state = 4;
                else
                {
                    current = NULL;
                    state = 0;
                }
                break;
            case 4:		// get column name or ')' end of table
                if (!strncmp(slask, "/*", 2))
                {
                    *comment = 0;
                    state = 7;
                }
                else if (!strcasecmp(slask, "primary") ||
                         !strcasecmp(slask, "key") ||
                         !strcasecmp(slask, "unique") ||
                         !strcasecmp(slask, "index") ||
                         !strcasecmp(slask, "constraint")
                        )
                {
                    // TODO: use index to build queries?
                    primary = !strcasecmp(slask, "primary") ? true : false;
                    unique = !strcasecmp(slask, "unique") ? true : false;
                    *index_name = 0;
                    *fields = 0;
                    state = 8;
                }
                else if (*slask == ')' || !strcmp(slask, ");"))
                {
                    if (!table -> primary)
                    {
                        if (table -> unique)
                        {
                            printf("Using unique index '%s' in table '%s' for update / erase methods\n",
                                   table -> unique -> name.c_str(),
                                   table -> name.c_str());
                            table -> primary = table -> unique;
                        }
                        else
                        {
                            printf("No primary / unique index on table '%s', update / erase methods won't be created\n",
                                   table -> name.c_str());
                        }
                    }
                    current = NULL;
                    state = 0;
                }
                else
                {
                    if (*slask == '`' || *slask == '"' || *slask == '\'')
                    {
                        memcpy(slask, slask + 1, strlen(slask + 1) + 1);
                        slask[strlen(slask) - 1] = 0;
                    }
                    strcpy(current_column, slask);
                    current_type = "";
                    uns = false;
                    pri = false;
                    auto_increment = false;
                    state = 5;
                }
                break;
            case 5:		// get column type
                current_type = slask;
                state = 51;
                wait_for = false;
                break;
            case 51:		// wait for ',' or ')'
                if (*slask == '(')
                    wait_for = true;
                if (wait_for)
                    current_type += slask;
                if (*slask == ')' && wait_for)
                    wait_for = false;
                else if (!wait_for && (*slask == ',' || *slask == ')'))
                {
                    current = reg_column(table, current_column, current_type.c_str(), uns);
                    if (auto_increment)
                        current -> ignore = true;
                    if (pri)
                    {
                        // create primary index with this column only
                        INDEX *p = new INDEX("", pri, false);
                        p -> fields.push_back(current);
                        p -> ambig = (primary || unique) ? check_index(table, p) : false;
                        table -> index.push_back(p);
                        table -> primary = p;
                    }
                    if (*slask == ',')
                        state = 4;
                    else if (*slask == ')') // end of create table
                    {
                        if (!table -> primary)
                        {
                            if (table -> unique)
                            {
                                printf("Using unique index '%s' in table '%s' for update / erase methods\n",
                                       table -> unique -> name.c_str(),
                                       table -> name.c_str());
                                table -> primary = table -> unique; // we want update / erase
                            }
                            else
                            {
                                printf("No primary / unique index on table '%s', update / erase methods won't be created\n",
                                       table -> name.c_str());
                            }
                        }
                        current = NULL;
                        state = 0;
                    }
                }
                else if (!strcasecmp(slask, "unsigned"))
                    uns = true;
                else if (!strcasecmp(slask, "primary"))
                    pri = true;
                else if (!strncasecmp(slask, "auto", 4))
                    auto_increment = true;
                break;
            case 6:		// wait for ',' or ')'
                if (*slask == ',')
                    state = 4;
                else if (*slask == ')')
                {
                    if (!table -> primary)
                    {
                        if (table -> unique)
                        {
                            printf("Using unique index '%s' in table '%s' for update / erase methods\n",
                                   table -> unique -> name.c_str(),
                                   table -> name.c_str());
                            table -> primary = table -> unique; // we want update / erase
                        }
                        else
                        {
                            printf("No primary / unique index on table '%s', update / erase methods won't be created\n",
                                   table -> name.c_str());
                        }
                    }
                    current = NULL;
                    state = 0;
                }
                break;
            case 7:
                while (strcmp(slask + strlen(slask) - 2, "*/"))
                {
                    if (*comment)
                        strcat(comment, " ");
                    strcat(comment, slask);
//					pa -> getword(slask);
                    get_next_token(slask);
                }
//				printf("comment: '%s'<br>\n",comment);
                if (current)
                    strcpy(current -> comment, comment);
                state = 4;
                break;
            case 8: // index
                if (!strcasecmp(slask, "primary"))
                    primary = true;
                else if (!strcasecmp(slask, "unique"))
                    unique = true;
                else if (!strcasecmp(slask, "key") || !strcasecmp(slask, "index"))
                {
                }
                else if (*slask == '(') // fields
                {
                    strcpy(fields, slask);
                    if (slask[strlen(slask) - 1] == ')')
                    {
                        parse_index(table, primary, unique, index_name, fields);
                        /*
                        printf("parse_index('%s',%s,%s,'%s','%s')\n",
                        	table -> name,
                        	primary ? "PRIMARY" : "",
                        	unique ? "UNIQUE" : "",
                        	index_name,
                        	fields);
                        */
                        state = 6;
                    }
                    else
                    {
                        state = 9;
                    }
                }
                else // index name
                {
                    strcpy(index_name, slask);
                }
                break;
            case 9: // get index fields
                strcat(fields, slask);
                {
                    int lp = 0;
                    int rp = 0;
                    for (size_t i = 0; i < strlen(fields); i++)
                    {
                        lp += (fields[i] == '(') ? 1 : 0;
                        rp += (fields[i] == ')') ? 1 : 0;
                    }
                    if (lp == rp)
                    {
                        parse_index(table, primary, unique, index_name, fields);
                        state = 6;
                    }
                }
                break;
            }
            //
            get_next_token(slask);
        }
//		delete pa;
        fgets(the_str, 30000, fil);
    }
    fclose(fil);


// generate source

    /*
    	if (makerel)
    	{
    		if (listlen(tblsbase) != 3)
    		{
    			fprintf(stderr,"You need exactly three tables to generate this.\n");
    			exit(-1);
    		}
    		// tbl3: [num] [ptrtbl1] [ptrtbl2] [valuex] [valuey] [charz]

    //		makerelclass(tblsbase,dbname);
    	} else
    */
    if (*libname || make_java)
    {
        generate_lib(overwrite, dbname, global, includedir, libname, prefix);
        if (build && !global)
        {
            printf("Won't build if it's not -global\n");
        }
#ifdef _WIN32
        printf("-build not implemented on _win32\n");
#else
        if (build && global)
        {
            sprintf(slask, "%s/src/%s", prefix, libname);
            if (!chdir(slask))
            {
                system("make install");
            }
            else
            {
                printf("chdir('%s') failed\n", slask);
            }
        }
#endif
    }
    else
    {
        printf("Warning. Using obsolete code generator - please use '-lib' option\n");
        for (tbls_l::iterator it = tblsbase.begin(); it != tblsbase.end(); it++)
        {
            TBLS *table = *it;
            generate_class(table, overwrite, dbname, global, includedir);
        }
    }
    return 0;
}

