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
#ifndef _LS_APIS_H
#define _LS_APIS_H

// if compiling with MSVC++ use its protection against multiple inclusion
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// includes
//
#include "lsapi_location.h"


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// function prototypes (and function pointers)
//
void InitAPIs();

extern int (*_GetRCCoordinate)(LPCSTR property, int defaultVal, int maxVal);
extern BOOL (WINAPI *_LSLog)(int nLevel, LPCSTR pszModule, LPCSTR pszMessage);
extern BOOL (WINAPIV *_LSLogPrintf)(int nLevel, LPCSTR pszModule, LPCSTR pszFormat, ...);
extern void (*_LSSetVariable)(LPCSTR var, LPCSTR value);


#endif // _LS_APIS_H