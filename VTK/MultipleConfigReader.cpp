/*

	MultipleConfigReader

	Author:	Vendicator
	
	Description:
	Class that handles the reading of *lines from .rc files
	and stores them for processing.

	Requires:

	Changelog:
	[2003-07-16 - Vendicator]
	- Checking for, and appending * to start of of Settingsname if not found.

	[2003-06-12 - Vendicator]
	- Using SafeString class

	[2003-03-22 - Vendicator]
	- First work?
	
*/

#include "lsapi_location.h"
#include "apis.h"
#include "MultipleConfigReader.h"

extern LPCSTR szLogName;


MultipleConfigReader::MultipleConfigReader(LPCTSTR configname, bool log)
{
	if ( _tcsncmp(configname, TEXT("*"), 1 ) != 0 )
	{
		configName.assign(TEXT("*"));
		configName.append(configname);
	}
	else
		configName.assign(configname);

	ReadConfigs(log);

	ResetConfigIterator();
}

MultipleConfigReader::MultipleConfigReader(LPCTSTR prefix, LPCTSTR config, bool log)
{
	if ( _tcsncmp(prefix, TEXT("*"), 1) != 0 )
	{
		configName.assign(TEXT("*"));
		configName.append(prefix);
	}
	else
		configName.assign(prefix);

	configName.append(config);

	ReadConfigs(log);

	ResetConfigIterator();
}

MultipleConfigReader::~MultipleConfigReader()
{
	PurgeConfigs();
#ifdef _DEBUG
	_LSLog(LOG_DEBUG, szLogName, "MultipleConfigReader Deleted");
#endif
}

void MultipleConfigReader::PurgeConfigs()
{
	// remove all configs entered
	for(Configs::iterator i = ConfigLines.begin(); i != ConfigLines.end(); i++)
	{
		delete (*i);
	}
	ConfigLines.clear();
}

void MultipleConfigReader::ResetConfigIterator()
{
	iter = ConfigLines.begin();
}

void MultipleConfigReader::ReReadConfigs(bool log)
{
	PurgeConfigs();

	ReadConfigs(log);

	ResetConfigIterator();
}

bool MultipleConfigReader::HasMoreConfigs()
{
	return (iter != ConfigLines.end());
}

// gets the configs that have been read, or NULL for end
LPCTSTR MultipleConfigReader::GetNextConfig()
{
	LPCTSTR configline = NULL;
	if (iter != ConfigLines.end())
	{
		// return this config
		configline = (*iter)->c_str();
		// get ready to return the next one on next invocation
		iter++;
	}
	return configline;
}

void MultipleConfigReader::ReadConfigs(bool log)
{
	FILE *steprc;
	TCHAR linebuffer[4096];
	SafeString* config;

#ifdef _DEBUG
	_LSLogPrintf(LOG_DEBUG, szLogName, "Looking for \"%s\" ", configName.char_str());
#endif

	steprc = LCOpen(NULL);
	if (steprc)
	{
		while ( LCReadNextConfig(steprc, configName.char_str(), linebuffer, sizeof(linebuffer)-1) )
		{
			config = new SafeString(linebuffer);
#ifdef _DEBUG
				_LSLogPrintf(LOG_DEBUG, szLogName, "found configline: %s", config->char_str());
#endif

			// insert the lines found
			ConfigLines.push_back(config);
		}
		LCClose(steprc);
	}
	else
	{
		_LSLog(LOG_ERROR, szLogName, "can't open .rc file for reading");
	}
}
