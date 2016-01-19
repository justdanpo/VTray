#ifndef __CLICKABLE_H
#define __CLICKABLE_H

#include <windows.h>
#include "SafeString.h"

#include "Action.h"
#include <map>
using std::map;
typedef map<int,Action*> ActionsList;

enum MouseKey {
	LEFT_DBL = WM_LBUTTONDBLCLK,
	LEFT_DOWN = WM_LBUTTONDOWN,
	LEFT_UP = WM_LBUTTONUP,
	RIGHT_DBL = WM_RBUTTONDBLCLK,
	RIGHT_DOWN = WM_RBUTTONDOWN,
	RIGHT_UP = WM_RBUTTONUP,
	MIDDLE_DBL = WM_MBUTTONDBLCLK,
	MIDDLE_DOWN = WM_MBUTTONDOWN,
	MIDDLE_UP = WM_MBUTTONUP
};

class Clickable
{
public:
	Clickable();
	Clickable(LPCTSTR prefix);
	~Clickable();

	void SetPrefix(LPCTSTR prefix);

	void ReadClickSettings();
	bool MouseExecute(Message& message);
	void SetMouseKey(const int ID, Action* new_action);
	void SetMouseKey(const int ID, LPCTSTR action);
	virtual HWND GetModuleWindow() = 0;

private:
	ActionsList actions;

	SafeString Prefix;
};

#endif