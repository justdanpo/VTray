#include "HideSettingsList.h"
#include <time.h>
#include "../VTK/apis.h"

HideSetting::HideSetting(SafeString cname, SafeString text, int hide) :
	TrayIconSetting(cname, text),
	hidetime(hide)
{
}

HideSetting::~HideSetting()
{
}

int HideSetting::GetTimer()
{
	return hidetime;
}

void HideSettingsList::AddSetting(SafeString cname, SafeString text, int hide)
{
	HideSetting* temp = new HideSetting(cname, text, hide);

	settings.insert(make_pair(HideSetting::GetCounter(), temp));
}

int HideSettingsList::GetTimer(int matchpos)
{
	HideSetting* temp;
	if (matchpos != MATCH_NONE)
	{
		SETTINGSLIST::iterator i = settings.find(matchpos);
		// and if we can find it again
		if (i != settings.end())
		{
			temp = (HideSetting*) i->second;
			return temp->GetTimer();
		}
	}
	// this shouldn't happen
	return 0;
}

int HideSettingsList::GetTimer(TrayIcon* pTrayIcon)
{
	return GetTimer(pTrayIcon->setting.MatchHidePos);
}


// measures time since last update if icon is able to hide
bool HideSettingsList::CheckIconTimeout(TrayIcon* pTrayIcon)
{
	// if classname was matched, tip is matched and timer has gone out..
	if ( pTrayIcon->setting.canHide && time(NULL) >= (GetTimer(pTrayIcon))*60+pTrayIcon->state.updated )
		return true;
	else
		return false;
}

bool HideSettingsList::MatchTrayIconTitle(TrayIcon* pTrayIcon)
{
	return TrayIconSettingsList::MatchTrayIconTitle(pTrayIcon, pTrayIcon->setting.MatchHidePos);
}