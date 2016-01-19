#ifndef __HOVER_H
#define __HOVER_H

#include <windows.h>
#include "SafeString.h"

enum {
	HOVER_OUT,
	HOVER_IN,
	HOVER_OVER
};


class Hover
{
public:
	Hover(LPCTSTR prefix = NULL);
	~Hover();

	void HoverCheck();
	void HoverCheck(POINT* point);
	void ReadHoverSettings();
	void PurgeHoverSettings();

	void SetMouseEnterCommand(SafeString enter);
	void SetMouseLeaveCommand(SafeString leave);

#ifdef FAKE_HWND
	virtual HWND GetModuleWindow();
#else
	virtual HWND GetModuleWindow() = 0;
#endif

protected:
	void HoverExecute(const int over);

protected:
	bool bHover;
	LPCTSTR Prefix;

	SafeString OnMouseEnterCommand;
	SafeString OnMouseLeaveCommand;

};

#endif