#ifndef __LSUTILS_H
#define __LSUTILS_H

#include <windows.h>
#include "lsapi_location.h"
#include "SafeString.h"

/*

EXAMPLE:

const Bang BangList[] =
{
	{ "!WndForeground", BangWndForeground }
};

// calculate how many bangs there are
const int NrOfBangs = sizeof(BangList) / sizeof(BangList[0]);

 */

namespace LSUtils
{
	struct Bang
	{
		LPCTSTR bangName;
		BangCommand* bangFunc;
	};

	// to default to maxline length...
	BOOL PrefixedGetRCLine(LPCTSTR prefix, LPCTSTR setting, LPTSTR buffer, LPCTSTR defaultVal);
	BOOL PrefixedGetRCLine(LPCTSTR prefix, LPCTSTR setting, LPTSTR buffer, UINT maxLen, LPCTSTR defaultVal);
	BOOL PrefixedGetRCString(LPCTSTR prefix, LPCTSTR setting, LPTSTR buffer, LPCTSTR defaultString, int maxLen);
	BOOL PrefixedGetRCBool(LPCTSTR prefix, LPCTSTR setting, bool def);
	int PrefixedGetRCInt(LPCTSTR prefix, LPCTSTR setting, int nDefault);
	int PrefixedGetRCCoordinate(LPCTSTR prefix, LPCTSTR setting, int nDefault, int maxVal);
	COLORREF PrefixedGetRCColor(LPCTSTR prefix, LPCTSTR setting, COLORREF colDef);
	void PrefixedLSSetVariable(LPCTSTR prefix, LPCTSTR var, LPCSTR value);

	void InsertBangs(const Bang banglist[], const int nrofbangs);
	void PrefixedInsertBangs(LPCTSTR prefix, const Bang banglist[], const int nrofbangs);
	void RemoveBangs(const Bang banglist[], const int nrofbangs);
	void PrefixedRemoveBangs(LPCTSTR prefix, const Bang banglist[], const int nrofbangs);
	HINSTANCE LoggedLSExecute(LPCTSTR szLogName, HWND Owner, LPCTSTR szCommand, int nShowCmd);
	HINSTANCE _LSExecute(HWND Owner, SafeString& sCommand, int nShowCmd);

	LPTSTR AppendSetting(LPCTSTR prefix, LPCTSTR setting, LPTSTR& buffer);

	/*class PrefixedSettingsReader
	{
	public:
		PrefixedSettingsReader(LPCTSTR prefix);

		BOOL GetRCLine(LPCSTR setting, LPSTR buffer, LPCSTR defaultVal);
		BOOL GetRCLine(LPCSTR setting, LPSTR buffer, UINT maxLen, LPCSTR defaultVal);
		BOOL GetRCString(LPCSTR setting, LPSTR buffer, LPCSTR defaultString, int maxLen);
		BOOL GetRCBool(LPCSTR setting, bool def);
		int GetRCInt(LPCSTR setting, int nDefault);
		int GetRCCoordinate(LPCSTR setting, int nDefault, int maxVal);
		COLORREF GetRCColor(LPCSTR setting, COLORREF colDef);
	protected:
		LPTSTR szPrefix;
	};*/
}

#endif