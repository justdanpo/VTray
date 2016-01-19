#ifndef __BANGMANAGER_H
#define __BANGMANAGER_H

#include <map>
#include <windows.h>
#include "lsapi_location.h"
#include "SafeString.h"

#pragma warning(disable:4786)


class BangManager
{
public:
	BangManager();
	~BangManager();

	void AddBang(LPCSTR bangName, BangCommand* func);

	void RemoveBang(LPCSTR bangName);

	void InsertBangs();

	void RemoveAllBangs();
	
protected:
	typedef std::map<SafeString, void*> BangStore;
	BangStore bangs;
	bool bInserted;

};

#endif
