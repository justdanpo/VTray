#include "TrayIconSettingsList.h"

#ifdef _MSC_VER // if using Microsoft C++ compiler
#pragma warning( disable : 4786 ) // disable warning C4786
#endif

class FlashSetting : public TrayIconSetting
{
public:
	FlashSetting(SafeString cname, SafeString text, int flash);
	~FlashSetting();
	int GetMaxFlash();

protected:
	int maxflash;

};

class FlashSettingsList : public TrayIconSettingsList
{
public:
	void AddSetting(SafeString cname, SafeString text, int flash);
	int GetMaxFlash(int matchpos);
	int GetMaxFlash(TrayIcon* pTrayIcon);
	bool MatchTrayIconTitle(TrayIcon* pTrayIcon);
};