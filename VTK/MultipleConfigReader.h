#ifndef __MULTIPLECONFIGREADER_H
#define __MULTIPLECONFIGREADER_H

#include <windows.h>
#include "SafeString.h"

#include <vector>
using namespace std;

typedef vector<SafeString*> Configs;

const char LogName[] = "MultipleConfigReader";

class MultipleConfigReader
{
public:
	MultipleConfigReader(LPCTSTR configname, bool log = false);
	MultipleConfigReader(LPCTSTR prefix, LPCTSTR config, bool log = false);
	~MultipleConfigReader();

	void PurgeConfigs();
	void ReReadConfigs(bool log = false);

	LPCTSTR GetNextConfig();
	void ResetConfigIterator();
	bool HasMoreConfigs();

protected:
	void ReadConfigs(bool log);

	Configs ConfigLines;
	SafeString configName;
	Configs::iterator iter;
};


#endif