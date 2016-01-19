#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H


#include <windows.h>

class WaitableObject
{
public:
	virtual BOOL Signal() = 0;
	virtual void Wait() = 0;
};

class Semaphore : public WaitableObject
{
public:
	Semaphore(LPCSTR name, int max_msgs, int start);
	~Semaphore();
	
	BOOL Signal();
	void Wait();

protected:
	HANDLE hSemaphore;
	char* sem_name;
};


class Mutex : public WaitableObject
{
public:
	Mutex(LPCSTR name);
	~Mutex();
	
	BOOL Signal();
	void Wait();

protected:
	HANDLE hMutex;
	char* mut_name;
};

#endif
