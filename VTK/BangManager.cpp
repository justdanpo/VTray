/*

	BangManager

	Author:	Vendicator

	Description:
	Simple class for handling the adding/removal of bangs into LiteStep.

	Requires:

	Changelog:
	[2003-07-16 - Vendicator]
	- Moved the simpler structs and functions to LSUtils

	[2003-05-28 - Vendicator]
	- First work?

*/

#include "BangManager.h"


BangManager::BangManager() :
	bInserted(false)
{
}

BangManager::~BangManager()
{
	if (bInserted)
		RemoveAllBangs();
}

void BangManager::AddBang(LPCSTR bangName, BangCommand* func)
{
	SafeString temp;
	temp.assign(bangName);
	bangs.insert(std::make_pair(bangName, (void*)func));
}

void BangManager::RemoveBang(LPCSTR bangName)
{
	for (BangStore::iterator i = bangs.begin(); i != bangs.end(); ++i)
	{
		if ( (*i).first == bangName )
			bangs.erase(i);
	}
}

void BangManager::InsertBangs()
{
	for (BangStore::iterator i = bangs.begin(); i != bangs.end(); ++i)
	{
		BangCommand* bangFunc = (BangCommand*) (*i).second;
		AddBangCommand( (*i).first.c_str(), bangFunc );
	}
	bInserted = true;
}

void BangManager::RemoveAllBangs()
{
	for (BangStore::iterator i = bangs.begin(); i != bangs.end(); ++i)
	{
		RemoveBangCommand( (*i).first.c_str() );
	}
	bInserted = false;
}