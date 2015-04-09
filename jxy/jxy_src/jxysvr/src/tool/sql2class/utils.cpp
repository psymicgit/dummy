/**
 **	File:	utils.cpp
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
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sql2class.h"

#include "utils.h"


#ifndef _WIN32
void strlwr(char *s)
{
	int i;

	for (i = 0; i < (int)strlen(s); i++)
		if (s[i] > 64 && s[i] <= 'Z')
			s[i] |= 32;
}

void strupr(char *s)
{
	int i;

	for (i = 0; i < (int)strlen(s); i++)
		if (s[i] > 96 && s[i] <= 'z')
			s[i] -= 32;
}
#endif

char *typestring(FIELDS *t)
{
static	char slask[200];

	if (!strcmp(t -> ctype,"double"))
		sprintf(slask,"double ");
	else
	if (!strcmp(t -> ctype,"long"))
		sprintf(slask,"long ");
	else
	if (!strcmp(t -> ctype,"text"))
		sprintf(slask,"char *");
	else
	if (*t -> ctype)
		sprintf(slask,"char *");
	else
		*slask = 0;

	return slask;
}

