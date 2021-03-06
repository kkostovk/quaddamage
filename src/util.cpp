/***************************************************************************
 *   Copyright (C) 2009-2015 by Veselin Georgiev, Slavomir Kaslev et al    *
 *   admin@raytracing-bg.net                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/**
 * @File util.cpp
 * @Brief a few useful short functions
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include "util.h"
using namespace std;

string upCaseString(string s)
{
	for (int i = 0; i < (int) s.length(); i++)
		s[i] = toupper(s[i]);
	return s;
}

string extensionUpper(const char* fileName)
{
	int l = (int) strlen(fileName);
	if (l < 2) return "";
	
	for (int i = l - 1; i >= 0; i--) {
		if (fileName[i] == '.') {
			string result = "";
			for  (int j = i + 1; j < l; j++) result += toupper(fileName[j]);
			return result;
		}
	}
	return "";
}

bool fileExists(const char* fn)
{
	char temp[512];
	strcpy(temp, fn);
	int l = (int) strlen(temp);
	if (l && temp[l - 1] == '/') temp[--l] = 0;
	struct stat st;
	return (0 == stat(temp, &st));
}
