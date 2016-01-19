#include "FlashSettingsList.h"

FlashSetting::FlashSetting(SafeString cname, SafeString text, int flash) :
	TrayIconSetting(cname, text),
	maxflash(flash)
{		
}

FlashSetting::~FlashSetting()
{
}

int FlashSetting::GetMaxFlash()
{
	return maxflash;
}

void FlashSettingsList::AddSetting(SafeString cname, SafeString text, int flash)
{
	FlashSetting* temp = new FlashSetting(cname, text, flash);

	settings.insert(make_pair(FlashSetting::GetCounter(), temp));
}

int FlashSettingsList::GetMaxFlash(int matchpos)
{
	FlashSetting* temp;
	if (matchpos != MATCH_NONE)
	{
		SETTINGSLIST::iterator i = settings.find(matchpos);
		// and if we can find it again
		if (i != settings.end())
		{
			temp = (FlashSetting*) i->second;
			return temp->GetMaxFlash();
		}
	}
	// this shouldn't happen
	return -1;
}

int FlashSettingsList::GetMaxFlash(TrayIcon* pTrayIcon)
{
	return GetMaxFlash(pTrayIcon->setting.MatchFlashPos);
}

bool FlashSettingsList::MatchTrayIconTitle(TrayIcon* pTrayIcon)
{
	return TrayIconSettingsList::MatchTrayIconTitle(pTrayIcon, pTrayIcon->setting.MatchFlashPos);
}