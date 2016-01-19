#include <conio.h>
#include <stdio.h>
//#include <string.h>
#include "SafeString.h"
//#include "BangManager.h"

LPCSTR szLogName = "temp";

void main()
{
	SafeString* test = new SafeString;
	test->assign("test");
	printf("1, %s, ok: %d \n", test->c_str(), *test == "test");
	
	test->append("ad");

	printf("3, %s ", test->c_str());
	delete test;

	getch();
}