#ifndef __COUNTINGBANGRUNNER_H
#define __COUNTINGBANGRUNNER_H

#ifdef _MSC_VER // if using Microsoft C++ compiler
#pragma warning( disable : 4786 ) // disable warning C4786
#endif

#include <windows.h>
#include "SafeString.h"
#include <map>
using namespace std;

#define BangMap map<int, SafeString> 

class CountingBangRunner
{
public:
	CountingBangRunner(HWND executeHwnd = NULL);
	~CountingBangRunner();

	void SetOnConfigs(SafeString onaddcmd, SafeString ondelcmd);
	void SetOnAddConfig(SafeString onaddcmd);
	void SetOnDelConfig(SafeString onaddcmd);
	void AddNrConfig(int nr, SafeString cmd);
	void SetExecutingHWND(HWND executeHwnd);

	int GetConfigCount();

	void PurgeNrCommands();
	void PurgeAll();

	void RunNext(int nr = -1);
	void RunPrevious(int nr = -1);

private:
	BangMap numberedCommands;
	HWND executingHWND;
	SafeString onAddCommand;
	SafeString onDelCommand;

	int counter;
};

#endif