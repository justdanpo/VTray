/*

	TokenSafeString

	Author:	Vendicator

	Description:
	This adds tokenizing to the SafeString class

	Changelog:
	[2003-07-28 - Vendicator]
	- Added CountTokens() to count how many tokens there are.

	[2003-07-10 - Vendicator]
	- Fixed an error if the tokenization hadn't been used when
	object is destroyed again.

	[2003-07-01 - Vendicator]
	- First work.

*/

#ifndef __TOKENSAFESTRING_H
#define __TOKENSAFESTRING_H

#include "lsapi_location.h"
#include "SafeString.h"
#include <tchar.h>

#define MAX_STRING_LENGTH STRSAFE_MAX_CCH - 1
#define MAX_NUMBER_LENGTH 64

#ifndef _T
#define _T(str) TEXT(str)
#endif

class TokenSafeString : public SafeString
{
public:
	TokenSafeString() :
		nTokens(0),
		tokens(NULL)
	{
	}

	TokenSafeString(LPCTSTR str) :
		SafeString(str),
		nTokens(0),
		tokens(NULL)
	{
		assign(str);
	}

	TokenSafeString(size_t size) :
		SafeString(size),
		nTokens(0),
		tokens(NULL)
	{
	}

	~TokenSafeString()
	{
		ClearTokens();
	}

	int CountTokens()
	{
#ifdef _DEBUG
		LSLogPrintf(LOG_DEBUG, "tokenize", "string: %s", str_data);
#endif		
		int len = length();
		if (len > 0)
		{
			int count = 0;
			for (int i = 0; i < len; ++i)
			{
				if (str_data[i] == ' ')
					++count;
			}
			return Tokenize(count+1);
		}
		else
			return 0;		
	}

#define TOKENSIZE 512
	int Tokenize(int nr_tokens)
	{
		if (nTokens != 0)
			ClearTokens();

		nTokens = nr_tokens;
		tokens = new char*[nTokens];
		for (int i = 0; i < nTokens; ++i)
		{
			tokens[i] = new char[TOKENSIZE];
		}
		
		extra_text[0] = '\0';

		int count = ::LCTokenize(char_str(), tokens, nr_tokens, extra_text);		
#ifdef _DEBUG
		for (i = 0; i < nTokens; ++i)
		{
			LSLogPrintf(LOG_DEBUG, "tokenize", "token %d: %s", i, tokens[i]);
			
		}		
#endif
		return count;
	}

	char* GetToken(int nr)
	{
		if (nr >= 0 && nr < nTokens)
		{
			return tokens[nr];
		}
		else
			return NULL;
	}

	char* GetExtra()
	{
		return extra_text;
	}

	void ClearTokens()
	{
		for (int i = 0; i < nTokens; ++i)
		{
			delete [] tokens[i];
		}
		if (tokens != NULL)
			delete tokens;

		nTokens = 0;
	}

protected:
	char extra_text[MAX_LINE_LENGTH];
	char** tokens;
	int nTokens;

};

#endif