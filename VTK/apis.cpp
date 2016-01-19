/*
This is a part of a LiteStep module source code.

Copyright (C) 2003 ilmcuts

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

/*

	API Wrappers

	Author:	Ilmcuts

	Description:
	Wrappers for loading newer api functions while still working with old builds.
	
	Changelog:
	[2003-07-25 - Vendicator]
	- Notification only for debug builds.

	[2003-07-09 - Vendicator]
	- Added MessageBox notification when using commands without running InitApis first.

*/

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// includes
//
#include "apis.h"

BOOL initialized = FALSE;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// local function prototypes
//
int GetRCCoordinateDummy(LPCSTR property, int defaultVal, int maxVal);
BOOL WINAPIV LSLogPrintfDummy(int nLevel, LPCSTR pszModule,
                              LPCSTR pszFormat, ...);
BOOL WINAPI LSLogDummy(int nLevel, LPCSTR pszModule, LPCSTR pszMessage);
void LSSetVariableDummy(LPCSTR var, LPCSTR value);


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Globals (the function pointers)
//
int (*_GetRCCoordinate)(LPCSTR property, int defaultVal,
                        int maxVal) = GetRCCoordinateDummy;

BOOL (WINAPI *_LSLog)(int nLevel, LPCSTR pszModule,
                      LPCSTR pszMessage) = LSLogDummy;

BOOL (WINAPIV *_LSLogPrintf)(int nLevel, LPCSTR pszModule,
                             LPCSTR pszFormat, ...) = LSLogPrintfDummy;

void (*_LSSetVariable)(LPCSTR var, LPCSTR value) = LSSetVariableDummy;


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// InitAPIs
//
void InitAPIs()
{
    HMODULE hLsApi = GetModuleHandle("LSAPI.DLL");
	initialized = TRUE;

    if (!hLsApi)
    {
        return;
    }

    FARPROC fp = GetProcAddress(hLsApi, "GetRCCoordinate");
    if (fp)
    {
        _GetRCCoordinate = (int (*)(LPCSTR,int,int))fp;
    }
    
    fp = GetProcAddress(hLsApi, "LSLog");
    if (fp)
    {
        _LSLog = (BOOL (WINAPI*)(int,LPCSTR,LPCSTR))fp;
    }
	else
	{
		fp = GetProcAddress(hLsApi, "_LSLog@12");
		if (fp)
		{
			_LSLog = (BOOL (WINAPI*)(int,LPCSTR,LPCSTR))fp;
		}
	}

    fp = GetProcAddress(hLsApi, "LSLogPrintf");
    if (fp)
    {
        _LSLogPrintf = (BOOL (WINAPIV*)(int,LPCSTR,LPCSTR,...))fp;
    }
    
    fp = GetProcAddress(hLsApi, "LSSetVariable");
    if (fp)
    {
        _LSSetVariable = (void (*)(LPCSTR,LPCSTR))fp;
    }
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// LSLogPrintfDummy
//
int GetRCCoordinateDummy(LPCSTR property, int defaultVal, int maxVal)
{
#ifdef _DEBUG
	if (!initialized)
		MessageBox(NULL, TEXT("Apis have not been initialized, run InitApis()"), TEXT("Apis not initialized"), MB_OK);
#endif

    int nReturn = GetRCInt(property, defaultVal);
    return min(nReturn, maxVal);
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// LSLogPrintfDummy
//
BOOL WINAPIV LSLogPrintfDummy(int /* nLevel */, LPCSTR /* pszModule */,
                              LPCSTR /* pszFormat */, ...)
{
#ifdef _DEBUG
	if (!initialized)
		MessageBox(NULL, TEXT("Apis have not been initialized, run InitApis()"), TEXT("Apis not initialized"), MB_OK);
#endif

    return TRUE;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// LSLogDummy
//
BOOL WINAPI LSLogDummy(int /* nLevel */, LPCSTR /* pszModule */,
                       LPCSTR /* pszMessage */)
{
#ifdef _DEBUG
	if (!initialized)
		MessageBox(NULL, TEXT("Apis have not been initialized, run InitApis()"), TEXT("Apis not initialized"), MB_OK);
#endif

    return TRUE;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// LSSetVariableDummy
//
void LSSetVariableDummy(LPCSTR /* var */, LPCSTR /* value */)
{
#ifdef _DEBUG
	if (!initialized)
		MessageBox(NULL, TEXT("Apis have not been initialized, run InitApis()"), TEXT("Apis not initialized"), MB_OK);
#endif
}
