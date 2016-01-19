#include "TrayIconSettingsList.h"
#include "../VTK/apis.h"
#include "Defines.h"

int TrayIconSetting::nUniqueCounter = 0;
extern LPCSTR szLogName;

TrayIconSetting::TrayIconSetting(SafeString cname, SafeString text)
{
	nUniqueCounter++;

	classname = cname;
	title = text;
}

TrayIconSetting::~TrayIconSetting()
{
}

// static function
int TrayIconSetting::GetCounter()
{
	return nUniqueCounter;
}

SafeString TrayIconSetting::GetClassname()
{
	return classname;
}

SafeString TrayIconSetting::GetTitle()
{
	return title;
}



TrayIconSettingsList::~TrayIconSettingsList()
{
	ClearAll();
}

int TrayIconSettingsList::GetConfigCount()
{
	return settings.size();
}

void TrayIconSettingsList::ClearAll()
{
	for (SETTINGSLIST::iterator i = settings.begin(); i!= settings.end(); i++)
	{
		delete (i->second);
	}
	settings.clear();
}

// looks for icon in defined configs and returns it's id counter, MATCH_NONE if not found
int TrayIconSettingsList::MatchTrayIconClass(TrayIcon* pTrayIcon)
{
	TCHAR classname[MAXCLASSNAME];

	GetClassName(pTrayIcon->hOwnerWnd, classname, MAXCLASSNAME-1 );
	SafeString storedname;

	for (SETTINGSLIST::iterator i = settings.begin(); i != settings.end(); i++)
	{
		storedname = (i->second->GetClassname());
		//_LSLogPrintf(LOG_DEBUG, szLogName, "testing classname \"%s\" with \"%s\" (%d)", classname, storedname.c_str(), storedname.length());
		if ( storedname.length() > 0 && _tcsstr(classname, storedname.c_str()) != NULL )
		{
#ifdef DEBUG_EXTRA
			_LSLogPrintf(LOG_DEBUG, szLogName, "Classname \"%s\" matched with \"%s\" (%d), saving config position", classname, storedname.c_str(), storedname.length());
			if (i->second->GetTitle().length())
			{
				_LSLog(LOG_DEBUG, szLogName, "tooltip != NULL");
			}
			else
			{
				_LSLog(LOG_DEBUG, szLogName, "no tip specified");
			}
#endif
			// return it's unique id
			return i->first;
		}
	}
	return MATCH_NONE;
}

/* 
	checks icons title against configs title (if set),
	wants to know the id of the config since this is a general function for both hide/flash
*/
bool TrayIconSettingsList::MatchTrayIconTitle(TrayIcon* pTrayIcon, const int matchpos)
{
	// if the icon knows where it's setting is
	if (matchpos != MATCH_NONE)
	{
		SETTINGSLIST::iterator i = settings.find(matchpos);
		// and if we can find it again
		if (i != settings.end())
		{
			//_LSLogPrintf(LOG_DEBUG, szLogName, "found position %d, checking title (%s, %s)", i->first, i->second->GetClassname().char_str(), i->second->GetTitle().char_str());
			// and we either don't have a title requirement, or we have a match in tooltips
			if ( i->second->GetTitle().length() <= 0 || strstr(pTrayIcon->szTip, i->second->GetTitle().char_str()) )
			{
				// then it's been matched
				return true;
			}			
		}
	}
	// otherwise it's not a title match
	return false;
}