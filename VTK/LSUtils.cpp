/*

	LSUtils

	Author:	Vendicator
	
	Description:
	Class that provides wrappers for reading settings with a prefix
	instead of supplying whole name (ie "Module", "Setting" for "ModuleSetting")

	Requires:

	Changelog:
	[2003-08-04 - Vendicator]
	- Renamed logging LSExecute, LSExecute for SafeStrings added.

	[2003-07-28 - Vendicator]
	- Proper logging of color reading.

	[2003-07-25 - Vendicator]
	- Fixed logging of LSSetvariable.

	[2003-07-16 - Vendicator]
	- Made as namespace, for using directive and shorter names.
	- Converted to tchar.
	- Moved InsertBangs/RemoveBangs here, added Prefixed versions.
	- Added PrefixedLSSetVariable

	[2003-06-12 - Vendicator]
	- Fixed some char* to LPCTSTR
	- Using _LSLog from apis.h

	[2002-11-09 - Vendicator]
	- First work?
	
*/

#include "apis.h"
#include "LSUtils.h"
#include "lsapi_location.h"

#include <tchar.h>

extern LPCSTR szLogName;


// Settings

BOOL LSUtils::PrefixedGetRCLine(LPCTSTR prefix, LPCTSTR setting, LPTSTR buffer, LPCTSTR defaultVal)
{
	BOOL res = FALSE;
	LPTSTR szTemp;
	AppendSetting(prefix, setting, szTemp);

	res = GetRCLine(szTemp, buffer, MAX_LINE_LENGTH, defaultVal);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "looking for setting: %s, got: %s", szTemp, buffer);
#endif
	delete [] szTemp;
	return res;
}

BOOL LSUtils::PrefixedGetRCLine(LPCTSTR prefix, LPCTSTR setting, LPTSTR buffer, UINT maxLen, LPCTSTR defaultVal)
{
	BOOL res = FALSE;
	LPTSTR szTemp;
	AppendSetting(prefix, setting, szTemp);
	
	res = GetRCLine(szTemp, buffer, maxLen, defaultVal);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "looking for setting: %s, got: %s", szTemp, buffer);
#endif
	delete [] szTemp;
	return res;
}

BOOL LSUtils::PrefixedGetRCString(LPCTSTR prefix, LPCTSTR setting, LPTSTR buffer, LPCTSTR defaultString, int maxLen)
{
	BOOL res = FALSE;
	LPTSTR szTemp;
	AppendSetting(prefix, setting, szTemp);
	res = GetRCString(szTemp, buffer, defaultString, maxLen);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "looking for setting: %s, got: %s", szTemp, buffer);
#endif
	delete [] szTemp;
	return res;
}

BOOL LSUtils::PrefixedGetRCBool(LPCTSTR prefix, LPCTSTR setting, bool def)
{
	BOOL retValue;
	LPTSTR szTemp;
	AppendSetting(prefix, setting, szTemp);
	retValue = GetRCBool(szTemp, def);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "looking for setting: %s, got %d", szTemp, retValue);
#endif
	delete [] szTemp;	
	return retValue;
}

int LSUtils::PrefixedGetRCInt(LPCTSTR prefix, LPCTSTR setting, int nDefault)
{
	LPTSTR szTemp;
	AppendSetting(prefix, setting, szTemp);
	int retValue = GetRCInt(szTemp, nDefault);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "looking for setting: %s, got %d", szTemp, retValue);
#endif
	delete [] szTemp;
	return retValue;
}

int LSUtils::PrefixedGetRCCoordinate(LPCTSTR prefix, LPCTSTR setting, int nDefault, int maxVal)
{
	LPTSTR szTemp;
	AppendSetting(prefix, setting, szTemp);
	int retValue = _GetRCCoordinate(szTemp, nDefault, maxVal);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "looking for setting: %s, got: %d", szTemp, retValue);
#endif
	delete [] szTemp;
	return retValue;
}

COLORREF LSUtils::PrefixedGetRCColor(LPCTSTR prefix, LPCTSTR setting, COLORREF colDef)
{
	LPTSTR szTemp;
	AppendSetting(prefix, setting, szTemp);
	COLORREF retValue = GetRCColor(szTemp, colDef);
#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "looking for setting: %s, got: %d,%d,%d", szTemp, GetRValue(retValue), GetGValue(retValue), GetBValue(retValue) );
#endif
	delete [] szTemp;
	return retValue;
}

void LSUtils::PrefixedLSSetVariable(LPCTSTR prefix, LPCTSTR var, LPCSTR value)
{
	LPTSTR szTemp;
	AppendSetting(prefix, var, szTemp);
	_LSSetVariable(szTemp, value);

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Setting var: %s, to: %s", szTemp, value);
#endif
	delete [] szTemp;
}

// Bang functions

HINSTANCE LSUtils::LoggedLSExecute(LPCTSTR szLogName, HWND Owner, LPCTSTR szCommand, int nShowCmd)
{
	_LSLogPrintf(LOG_DEBUG, szLogName, "Running Bang: %s", szCommand);
	return LSExecute(Owner, szCommand, nShowCmd);
}

HINSTANCE LSUtils::_LSExecute(HWND Owner, SafeString& sCommand, int nShowCmd)
{
	return LSExecute(Owner, sCommand.char_str(), 0);
}

void LSUtils::InsertBangs(const Bang banglist[], const int nrofbangs)
{
	if (banglist == NULL)
		return;

	for (int i = 0; i < nrofbangs; i++)
		AddBangCommand(banglist[i].bangName, *banglist[i].bangFunc);
}


void LSUtils::PrefixedInsertBangs(LPCTSTR prefix, const Bang banglist[], const int nrofbangs)
{
	if (banglist == NULL)
		return;

	LPTSTR szTemp;
	LPTSTR bangPrefix;
	bool prefixed = false;

	if ( _tcsncmp(prefix, TEXT("!"), 1 ) != 0)
	{
		AppendSetting(TEXT("!"), prefix, bangPrefix);
		prefixed = true;
	}

	for (int i = 0; i < nrofbangs; i++)
	{
		AppendSetting(bangPrefix, banglist[i].bangName, szTemp);
		AddBangCommand(szTemp, *banglist[i].bangFunc);
		delete [] szTemp;
	}
	if (prefixed)
		delete [] bangPrefix;
}

void LSUtils::RemoveBangs(const Bang banglist[], const int nrofbangs)
{
	if (banglist == NULL)
		return;

	for (int i = 0; i < nrofbangs; i++)
		RemoveBangCommand(banglist[i].bangName);
}

void LSUtils::PrefixedRemoveBangs(LPCTSTR prefix, const Bang banglist[], const int nrofbangs)
{
	if (banglist == NULL)
		return;

	LPTSTR szTemp;
	LPTSTR bangPrefix;
	bool prefixed = false;

	if ( _tcsncmp(prefix, TEXT("!"), 1 ) != 0)
	{
		AppendSetting(TEXT("!"), prefix, bangPrefix);
		prefixed = true;
	}

	for (int i = 0; i < nrofbangs; i++)
	{
		AppendSetting(bangPrefix, banglist[i].bangName, szTemp);
		RemoveBangCommand(szTemp);
		delete [] szTemp;
	}
	if (prefixed)
		delete [] bangPrefix;
}

LPTSTR LSUtils::AppendSetting(LPCTSTR prefix, LPCTSTR setting, LPTSTR& buffer)
{
	buffer = new TCHAR[_tcslen(prefix) + _tcslen(setting) + 1];
	_tcscpy(buffer, prefix);
	_tcscat(buffer, setting);
	return buffer;
}