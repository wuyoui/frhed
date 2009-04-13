/////////////////////////////////////////////////////////////////////////////
//    License (GPLv2+):
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful, but
//    WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
/////////////////////////////////////////////////////////////////////////////
/** 
 * @file  Registry.cpp
 *
 * @brief Implementation of Frhed registry functions.
 *
 */
// ID line follows -- this is updated by SVN
// $Id$

#include "precomp.h"
#include "version.h"
#include "Constants.h"
#include "Registry.h"
#include "shtools.h"
#include "regtools.h"

BOOL contextpresent()
{
	HKEY key1;
	LONG res = RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("*\\shell\\Open in Frhed\\command"), 0, KEY_READ, &key1);
	if (res == ERROR_SUCCESS) //succeeded check if has the required keys & data
	{
		TCHAR stringval[MAX_PATH];
		long len = sizeof stringval;
		RegQueryValue(key1, NULL, stringval, &len);
		PathRemoveArgs(stringval);
		PathUnquoteSpaces(stringval);
		RegCloseKey(key1);
		if (PathsEqual(stringval, _pgmptr) == 0)
			return 1;
	}
	return 0;
}

BOOL defaultpresent()
{
	TCHAR stringval[MAX_PATH];
	long len = sizeof stringval;
	LONG res = RegQueryValue(HKEY_CLASSES_ROOT, _T("Unknown\\shell"), stringval, &len);
	return res == ERROR_SUCCESS && _tcscmp(stringval, _T("Open in Frhed")) == 0;
}

BOOL unknownpresent()
{
	HKEY key1;
	LONG res = RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("Unknown\\shell\\Open in Frhed\\command"), 0, KEY_READ, &key1);
	if (res == ERROR_SUCCESS) //succeeded check if has the required keys & data
	{
		TCHAR stringval[MAX_PATH];
		long len = sizeof stringval;
		RegQueryValue(key1, NULL, stringval, &len);
		PathRemoveArgs(stringval);
		PathUnquoteSpaces(stringval);
		RegCloseKey(key1);
		if (PathsEqual(stringval, _pgmptr) == 0)
			return 1;
	}
	return 0;
}

BOOL oldpresent()
{
	HKEY hk;
	LONG res = RegOpenKeyEx(HKEY_CURRENT_USER, OptionsRegistryPath, 0, KEY_READ, &hk);
	if (res == ERROR_SUCCESS)
	{
		TCHAR subkeynam[MAX_PATH] = {0};
		for (DWORD i = 0; ; i++)
		{
			res = RegEnumKey(hk ,i, subkeynam, MAX_PATH);
			if (res == ERROR_NO_MORE_ITEMS)
				break;
			else if (0 != strcmp(subkeynam, OptionsRegistrySettingsPath))
			{
				RegCloseKey(hk);
				return TRUE;
			}
		}
		RegCloseKey(hk);
	}
	return FALSE;
}

/**
 * @brief Check if Frhed settings key already exist.
 */
BOOL frhedpresent()
{
	//Check if frhed\subreleaseno exists
	HKEY hk;
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, OptionsRegistrySettingsPath, 0, KEY_READ, &hk))
		return FALSE;
	RegCloseKey(hk);
	return TRUE;
}

/**
 * @brief Check if links key in Frhed settings already exist.
 */
BOOL linkspresent()
{
	HKEY hk;
	TCHAR keyname[64] = {0};
	_sntprintf(keyname, RTL_NUMBER_OF(keyname), _T("%s\\links"),
			OptionsRegistrySettingsPath);
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, keyname, 0, KEY_READ, &hk))
		return FALSE;
	RegCloseKey(hk);
	return TRUE;
}
