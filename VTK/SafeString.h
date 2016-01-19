/*

	SafeString

	Author:	Vendicator
	
	Description:
	The purpose of this class is to make a STL conformant string class
	using TCHAR and the safe string operation provided by strsafe.h

	Changelog:
	[2003-08-10 - Vendicator]
	Fixed misplaced mem deallocation which caused .append() to not work.

	[2003-08-09 - Vendicator]
	Fixed a possible memleak.
	Logging of allocations on debug.
	char_str() only converts when string has changed.
	Extra precaution when allocating memory.

	[2003-08-04 - Vendicator]
	Added operator [].

	[2003-07-10 - Vendicator]
	If not using unicode or multibyte an _ANSI flag is set to
	exclude the explicit char converting ( char_str() ).

	[2003-07-09 - Vendicator]
	Included operator= for SafeStrings as well.

	[2003-07-07 - Vendicator]
	Fixed assign of SafeStrings.
	Added sub string searches.

	[2003-07-02 - Vendicator]
	Added copy constructor for SafeStrings.

	[2003-06-16 - Vendicator]
	Added operator== for LPCTSTR comparison.
	Added operator< for map<> insertion.

	[2003-06-12 - Vendicator]
	- First work, added assign(), append(), c_str().
	
*/

#ifndef __SAFESTRING_H
#define __SAFESTRING_H

#include <windows.h>
#include <assert.h>

#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
#undef STRSAFE_NO_DEPRECATE
#include <tchar.h>

#define MAX_STRING_LENGTH STRSAFE_MAX_CCH - 1
#define MAX_NUMBER_LENGTH 64

#ifndef _T
#define _T(str) TEXT(str)
#endif

#ifndef _UNICODE
#ifndef _MCBS
#define _ANSI
#endif
#endif

#ifdef _DEBUG
	#define _DEBUG_LOG
	#include <stdio.h>
#endif


//#define __STL

#ifndef __STL

class SafeString
{
public:
	SafeString() :
		nLength(0),
		nSize(0),
		str_data(NULL)
#ifndef _ANSI
		,bUpdated(false)
		,str_char_data(NULL)
#endif
	{
#ifdef _DEBUG_LOG
		out = fopen("C:\\safestring_log.txt", "a");
#endif
	}

	SafeString(LPCTSTR str) :
		nLength(0),
		nSize(0),
		str_data(NULL)
#ifndef _ANSI
		,bUpdated(false)
		,str_char_data(NULL)
#endif
	{
#ifdef _DEBUG_LOG
		out = fopen("C:\\safestring_log.txt", "a");
#endif
		assign(str);
	}

	SafeString(size_t size) :
		nLength(0),
		nSize(size),
		str_data(NULL)
#ifndef _ANSI
		,bUpdated(false)
		,str_char_data(NULL)
#endif

	{
#ifdef _DEBUG_LOG
		out = fopen("C:\\safestring_log.txt", "a");
#endif
		AllocateSize(size);
		size_t byte_length;
		
		if ( SUCCEEDED(StringCbLength(str_data, size * sizeof(TCHAR), &byte_length)) )
			memset(str_data, 0, byte_length);
		else
		{
			Cleanup();
		}
	}

	SafeString(const SafeString& str) :
		str_data(NULL)
#ifndef _ANSI
		,bUpdated(false)
		,str_char_data(NULL)
#endif
	{
#ifdef _DEBUG_LOG
		out = fopen("C:\\safestring_log.txt", "a");
#endif
		assign(str);
	}

	SafeString(const SafeString* str) :
		str_data(NULL)
#ifndef _ANSI
		,bUpdated(false)
		,str_char_data(NULL)
#endif
	{
#ifdef _DEBUG_LOG
		out = fopen("C:\\safestring_log.txt", "a");
#endif
		assign(*str);
	}

	~SafeString()
	{
#ifdef _DEBUG_LOG
		fclose(out);
#endif
		Cleanup();
		//_LSLog(LOG_DEBUG, szLogName, "SafeString Deleted");
	}

	SafeString& operator=(LPCTSTR str)
	{
		assign(str);
		return *this;
	}

	SafeString& operator=(const SafeString& str)
	{
		assign(str);
		return *this;
	}

	void assign(LPCTSTR str, size_t copy_length = 0)
	{
		size_t max_length = MAX_STRING_LENGTH;
		size_t length;
		HRESULT ok = S_OK;

		Cleanup();

		if (str == NULL)
		{
			assign(TEXT(""));
			return;
		}
		
		ok = StringCchLength(str, max_length, &length);

		if ( SUCCEEDED(ok) )
		{
			if ( copy_length > 0 )
			{
				// don't try to copy more than the inputed string length
				length = min(copy_length, length);
				AllocateSize(length);
				StringCchCopyN(str_data, length+1, str, length);
			}
			else
			{
				AllocateSize(length);
				StringCchCopy(str_data, length+1, str);
			}
#ifndef _ANSI
			bUpdated = true;
#endif
			nLength = length;
		}
	}

	void assign(const SafeString& str, size_t copy_length = 0)
	{
		size_t length = str.nLength;
		Cleanup();

		if ( copy_length > 0 )
		{
			// don't try to copy more than the inputed string length
			length = min(copy_length, length);
			AllocateSize(length);
			StringCchCopyN(str_data, length+1, str.str_data, length);
		}
		else
		{
			AllocateSize(length);
			StringCchCopy(str_data, length+1, str.str_data);
		}
#ifndef _ANSI
		bUpdated = true;
#endif
		nLength = length;
	}

	void append(LPCTSTR str)
	{
		LPTSTR new_str = NULL;

		size_t add_length;
		size_t new_length;

		if ( SUCCEEDED(StringCchLength(str, MAX_STRING_LENGTH, &add_length)) )
		{
			new_length = nLength + add_length;

			// if we need a bigger string container
			if (new_length > nSize)
			{
				// copy old string to new container
				AllocateSize(new_length, &new_str);

				// if there was something in the string before
				if (nLength > 0)
					StringCchCopy(new_str, new_length+1, str_data);				

				// free old memory
				Cleanup();

				// set up new container
				nLength = new_length;
				nSize = new_length;
				str_data = new_str;
			}

			// concatenate with adding string
			StringCbCat(str_data, new_length+1, str);

#ifndef _ANSI
			bUpdated = true;
#endif
		}
	}

	void append(SafeString& str)
	{
		LPTSTR new_str = NULL;

		size_t new_length;

		new_length = nLength + str.nLength;

		// if we need a bigger string container
		if (new_length > nSize)
		{
			// copy old string to new container
			AllocateSize(new_length, &new_str);

			// if there was something in the string before
			if (nLength > 0)
				StringCchCopy(new_str, new_length+1, str_data);				

			// free old memory
			Cleanup();

			// set up new container
			nLength = new_length;
			nSize = new_length;
			str_data = new_str;
		}

		// concatenate with adding string
		StringCbCat(str_data, new_length+1, str.str_data);

#ifndef _ANSI
		bUpdated = true;
#endif
	}

	void append(const int number)
	{
		TCHAR buffer[MAX_NUMBER_LENGTH+1];
		StringCchPrintf(buffer, MAX_NUMBER_LENGTH, _T("%d"), number);
		append(buffer);
	}

	void append(TCHAR letter)
	{
		TCHAR buffer[3];
		StringCchPrintf(buffer, 2, _T("%c"), letter);
		append(buffer);
	}

	size_t length() const
	{
		return nLength;
	}

	// either tchar string returned
	LPCTSTR c_str() const
	{
		return str_data;
	}

	// conversion to C char string, for use with lsapi etc.
	LPSTR char_str()
	{
#ifndef _ANSI
		if (bUpdated)
		{
			if (str_char_data != NULL)
				delete [] str_char_data;

			str_char_data = new char[nLength + 1];

			for (size_t i = 0; i < nLength; i++)
			{
				str_char_data[i] = (char) str_data[i];
			}
			str_char_data[nLength] = '\0';
		
			bUpdated = false;
		}
		return str_char_data;
#else
		return (LPSTR) str_data;
#endif
	}

	// sprintf assignment
	void formated_assign(LPCTSTR pszFormat, ...)
	{
		LPTSTR buffer;
		AllocateSize(MAX_STRING_LENGTH, &buffer);

		va_list argList;
		va_start(argList, pszFormat);
		StringCchPrintf(buffer, MAX_STRING_LENGTH, pszFormat, argList);
		va_end(argList);
		
		assign(buffer);
		delete [] buffer;
	}

	SafeString* find(LPCTSTR cstring) const
	{
		SafeString* str = NULL;
		LPTSTR found = _tcsstr(str_data, cstring);
		if (found)
			 str = new SafeString( found );

		return str;
	}

	// non standard substring search
	bool str_find(LPCTSTR cstring) const
	{
		return (_tcsstr(str_data, cstring) != NULL);
	}

	bool compare(LPCTSTR cstring) const
	{
		return (_tcscmp(str_data, cstring) == 0);
	}

	bool compare(const SafeString& cstring) const
	{
		return (_tcscmp(str_data, cstring.str_data) == 0);
	}

	bool operator== (LPCTSTR cstring) const
	{
		return compare(cstring);
	}

	bool operator== (const SafeString& cstring) const
	{
		return compare(cstring);
	}

	bool operator< (const SafeString cstring) const
	{
		return (_tcscmp(str_data, cstring.str_data ) < 0);
	}

	/*operator bool() const
	{
		return (nLength > 0);
	}*/

	TCHAR& operator[] (int index)
	{
		assert( index >= 0 && index < nLength );
		return str_data[index];
	}


protected:
	// frees memory for the string and any converted char[]
	void Cleanup()
	{
		if (str_data != NULL)
		{
#ifdef _DEBUG_LOG
			fprintf(out, "Cleaning up string: %d\n", str_data);
			fflush(out);
#endif
			delete [] str_data;
			str_data = NULL;
		}
#ifndef _ANSI
		if (str_char_data != NULL)
		{
			delete [] str_char_data;
			str_char_data = NULL;	
		}
#endif
		nLength = 0;
		nSize = 0;
	}

	// allocate size for a certain amount of chars
	void AllocateSize(size_t size, LPTSTR* str = NULL)
	{
		if (str == NULL)
		{
			// extra precaution
			Cleanup();

			nSize = size + 1;
			str_data = new TCHAR[size + 1];
#ifdef _DEBUG_LOG
			fprintf(out, "Allocating string: %d\n", str_data);
			fflush(out);
#endif
		}
		else
		{
			*str = new TCHAR[size + 1];
		}		
	}

protected:
#ifdef _DEBUG_LOG
	FILE* out;
#endif

	size_t nLength;
	size_t nSize;
	LPTSTR str_data;
#ifndef _ANSI
	bool bUpdated;
	LPSTR str_char_data;
#endif	

};

#else
	#include <string>
	using std::string;
	typedef string SafeString;
#endif //__STL

#endif