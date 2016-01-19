#ifndef __TRAYICONSETTINGSLIST_H
#define __TRAYICONSETTINGSLIST_H

#ifdef _MSC_VER // if using Microsoft C++ compiler
#pragma warning( disable : 4786 ) // disable warning C4786
#endif

#include "TrayIconSettingsList.h"
#include "TrayIcon.h"
#include "../VTK/SafeString.h"
#include <map>
using namespace std;

#define	MAXCLASSNAME	128

class TrayIconSetting
{
public:
	TrayIconSetting(SafeString cname, SafeString text);
	~TrayIconSetting();

	static int GetCounter();
	SafeString GetClassname();
	SafeString GetTitle();

protected:
	SafeString classname;
	SafeString title;

	static int nUniqueCounter;
};


typedef map<int,TrayIconSetting*>	SETTINGSLIST;

class TrayIconSettingsList
{
public:
	~TrayIconSettingsList();

	void ClearAll();
	int GetConfigCount();

	int MatchTrayIconClass(TrayIcon* pTrayIcon);
	virtual bool MatchTrayIconTitle(TrayIcon* pTrayIcon, const int matchpos);
	
protected:
	SETTINGSLIST settings;

};

#endif