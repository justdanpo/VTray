#include "TrayIconSettingsList.h"

#ifdef _MSC_VER // if using Microsoft C++ compiler
#pragma warning( disable : 4786 ) // disable warning C4786
#endif

class HideSetting : public TrayIconSetting
{
public:
	HideSetting(SafeString cname, SafeString text, int hide);
	~HideSetting();
	int GetTimer();

protected:
	int hidetime;

};

class HideSettingsList : public TrayIconSettingsList
{
public:
	void AddSetting(SafeString cname, SafeString text, int hide);
	int GetTimer(int matchpos);
	int GetTimer(TrayIcon* pTrayIcon);
	
	bool CheckIconTimeout(TrayIcon* pTrayIcon);
	bool MatchTrayIconTitle(TrayIcon* pTrayIcon);
};