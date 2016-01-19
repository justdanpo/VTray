/*

	CountingBangRunner

	Author:	Vendicator
	
	Description:
	Class for running counted commands, ie. for running commands on add/del of icons.

	Requires:
	SafeString

	Changelog:
	[2003-07-16 - Vendicator]
	- First work, split out from TrayIcon class.
	
*/

#include "CountingBangRunner.h"

#include "lsapi_location.h"
#include "apis.h"

extern LPCSTR szLogName; // = "CountingBangRunner";

CountingBangRunner::CountingBangRunner(HWND executeHwnd) :
	counter(0),
	executingHWND(executeHwnd)
{
}

CountingBangRunner::~CountingBangRunner()
{
	PurgeAll();
}

void CountingBangRunner::PurgeNrCommands()
{
	//for (BangMap::iterator i = numberedCommands.begin(); i != numberedCommands.end(); i++)
	//	delete [] (*i).second;

	numberedCommands.clear();
}

void CountingBangRunner::PurgeAll()
{
	PurgeNrCommands();
}

int CountingBangRunner::GetConfigCount()
{
	return numberedCommands.size();
}

void CountingBangRunner::SetExecutingHWND(HWND exeHwnd)
{
	executingHWND = exeHwnd;
}

void CountingBangRunner::AddNrConfig(int nr, SafeString cmd)
{
	numberedCommands.insert(make_pair(nr, cmd));
}

void CountingBangRunner::SetOnConfigs(SafeString onaddcmd, SafeString ondelcmd)
{
	SetOnAddConfig(onaddcmd);
	SetOnDelConfig(ondelcmd);
}

void CountingBangRunner::SetOnAddConfig(SafeString onaddcmd)
{
	onAddCommand = onaddcmd;
}

void CountingBangRunner::SetOnDelConfig(SafeString ondelcmd)
{
	onDelCommand = ondelcmd;
}

void CountingBangRunner::RunNext(int nr)
{
	counter++;

	if (counter != nr && nr >= 0)
	{
		_LSLogPrintf(LOG_NOTICE, szLogName, "(+) Counted bang nr different from requested: c %d r %d (changing to requested)", counter, nr);
		counter = nr;
	}

	// check if there is a bang command for this count and the run it
	BangMap::iterator find = numberedCommands.find(counter);
	if (find != numberedCommands.end())
	{
		_LSLogPrintf(LOG_DEBUG, szLogName, "Executing nr %d: \"%s\" ", counter, (*find).second.char_str());
		LSExecute(executingHWND, (*find).second.char_str(), NULL);
	}

	// run "on add" command
	if (onAddCommand.length() > 0)
	{
		_LSLogPrintf(LOG_DEBUG, szLogName, "Executing std add: \"%s\" ", onAddCommand.char_str());
		LSExecute(executingHWND, onAddCommand.char_str(), NULL);
	}
}

void CountingBangRunner::RunPrevious(int nr)
{
	counter--;

	if (counter != nr && nr >= 0)
	{
		_LSLogPrintf(LOG_NOTICE, szLogName, "(-) Counted bang nr different from requested: c %d r %d (changing to requested)", counter, nr);
		counter = nr;
	}

	// check if there is a bang command for this count and the run it
	BangMap::iterator find = numberedCommands.find(counter);
	if (find != numberedCommands.end())
	{
		_LSLogPrintf(LOG_DEBUG, szLogName, "Executing nr %d: \"%s\" ", counter, (*find).second.char_str());
		LSExecute(executingHWND, (*find).second.char_str(), NULL);
	}

	// run "on del" command
	if (onDelCommand.length() > 0)
	{
		_LSLogPrintf(LOG_DEBUG, szLogName, "Executing std del: \"%s\" ", onDelCommand.char_str());
		LSExecute(executingHWND, onDelCommand.char_str(), NULL);
	}
}