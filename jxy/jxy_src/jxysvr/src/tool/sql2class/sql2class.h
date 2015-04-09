/**
 **	File:	sql2class.h
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
#include <vector>
#include <string>
#include <list>

	typedef struct fieldsstruct
	{
		std::string GetCType(std::string& intyp,std::string& suffix,bool java = false);
		struct fieldsstruct *next;
		int num;
		char column[200];
		char typ[200];
		char ctype[200];
		int length;		// char length of ctype
		char comment[100];
		bool ignore;		// ignore field in insert/update
		bool uns;		// unsigned integer field
		int bitsize;
		bool enum_set;		// ENUM or SET type
		bool set;               // type is SET
		std::vector<std::string> mvec; // ENUM / SET values
		std::string sqlname;	// original sql column name
	} FIELDS;
	typedef std::vector<FIELDS *> fields_v;
	typedef std::list<FIELDS *> fields_l;

	struct INDEX
	{
		INDEX(const std::string& n,bool p,bool u) : name(n),primary(p),unique(u) {}
		std::string name;
		bool primary;
		bool unique;
		fields_v fields;
		bool ambig;
	};
	typedef std::vector<INDEX *> index_v;

	typedef struct tblsstruct
	{
		struct tblsstruct *next;
		std::string name;
		fields_l fieldsbase;
		index_v index;
		INDEX *primary;
		INDEX *unique;
	} TBLS;
	typedef std::list<TBLS *> tbls_l;

extern	tbls_l tblsbase;
extern	std::string namespc;
extern	short container;
extern	std::string baseclass;
extern	bool mediumtext; // vector<string> needed
extern	bool use_stl;
extern	bool use_sqlite; // use sqlite3 wrapper, not mysql
extern	bool make_java;
extern	std::string package;
extern	bool license;
extern	std::string queryclass;
extern	std::string cmdline;
extern	bool use_odbc; // use odbc wrapper, not mysql
extern	bool make_getset;
extern	bool mangle_names;
extern	bool use_wrapped; // use sqlwrapped wrapper (mysql/sqlite3/odbc), not mysql
