/*

	Clickable

	Author:	Vendicator
	
	Description:
	Simple class that handles clicking of window

	Requires:
	- GetModuleWnd() implemented in child class

	Changelog:
	[2003-06-12 - Vendicator]
	- Using SafeString class

	[2003-01-05 - Vendicator]
	- First work?
	
*/

#include "Clickable.h"
#include "LSUtils.h"
#include "lsapi_location.h"
#include "BangAction.h"

//#define DEBUG

Clickable::Clickable()
{
}

Clickable::Clickable(LPCTSTR prefix)
{
	Prefix.assign(prefix);
}

Clickable::~Clickable()
{
	for (ActionsList::iterator i = actions.begin(); i != actions.end(); ++i)
	{
		delete i->second;
	}
}

void Clickable::SetPrefix(LPCTSTR prefix)
{
	Prefix.assign(prefix);
}

void Clickable::SetMouseKey(const int ID, LPCTSTR action)
{
	BangAction* new_action = new BangAction(action);
	SetMouseKey(ID, new_action);
}

void Clickable::SetMouseKey(const int ID, Action* new_action)
{
	ActionsList::iterator found = actions.find(ID);
	if (found != actions.end())
	{
		delete found->second;
		actions.erase(found);
	}

	actions.insert( std::make_pair(ID, new_action) );
}

void Clickable::ReadClickSettings()
{
	char szTemp[MAX_LINE_LENGTH];
	memset(szTemp, 0, MAX_LINE_LENGTH);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnLeftDBL", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_LBUTTONDBLCLK, szTemp);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnLeftDown", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_LBUTTONDOWN, szTemp);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnLeftUp", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_LBUTTONUP, szTemp);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnRightDBL", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_RBUTTONDBLCLK, szTemp);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnRightDown", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_RBUTTONDOWN, szTemp);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnRightUp", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_RBUTTONUP, szTemp);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnMiddleDBL", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_MBUTTONDBLCLK, szTemp);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnMiddleDown", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_MBUTTONDOWN, szTemp);

	LSUtils::PrefixedGetRCLine(Prefix.c_str(), "OnMiddleUp", szTemp, MAX_LINE_LENGTH, "!None");
	SetMouseKey(WM_MBUTTONUP, szTemp);
}

bool Clickable::MouseExecute(Message& message)
{
	ActionsList::iterator found = actions.find(message.uMsg);

	if (found != actions.end())
	{
		found->second->Execute(message);
		return true;
	}
	else
		return false;
}