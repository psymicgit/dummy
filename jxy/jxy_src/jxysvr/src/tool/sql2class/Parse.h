/**
 **	Parse.h - parse a string
 **
 **	Written: 1999-Feb-10 grymse@alhem.net
 **/

/*
Copyright (C) 1999  Anders Hedstrom

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

/***************************************************/
/* interface of class Parse */

class Parse
{
public:
	Parse();
	Parse(char *);
	Parse(char *,char *);
	~Parse();
	short issplit(char);
	void getsplit(void);
	void getsplit(char *);
	void getword(void);
	void getword(char *);
	void getword(char *,char *,int);
	void getrest(char *);
	long getvalue(void);
	void setbreak(char);
	int getwordlen(void);
	int getrestlen(void);
	void enablebreak(char c) {
		pa_enable = c;
	}
	void disablebreak(char c) {
		pa_disable = c;
	}

private:
	char *pa_the_str;
	char *pa_splits;
	char *pa_ord;
	int   pa_the_ptr;
	char  pa_breakchar;
	char  pa_enable;
	char  pa_disable;
};

