#ifndef __BANGACTION_H
#define __BANGACTION_H

#include "Action.h"

class BangAction : public Action
{
public:
	BangAction(LPCTSTR command)
	{
		bang.assign(command);
	}

	~BangAction()
	{
	}

	static void ConnectWindow(HWND window)
	{
		hWnd = window;
	}

	void Execute(Message& message)
	{
		#ifdef DEBUG
			LSUtils::LoggedBangExecute(Prefix, GetModuleWindow(), bang.c_str(), 0);
		#else
			LSExecute(hWnd, bang.c_str(), 0);
		#endif
	}

protected:
	SafeString bang;
	static HWND hWnd;
};

HWND BangAction::hWnd;

#endif