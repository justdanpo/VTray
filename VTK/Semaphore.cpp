#include "Semaphore.h"


#define DEBUG

#ifdef DEBUG
#include <stdio.h>

FILE* out = fopen("C:\\wait_log.txt", "w");
int msg_count;
#endif

Semaphore::Semaphore(LPCSTR name, int max_msgs = 10, int start = 0)
{
	sem_name = new char[strlen(name)+1];
	strcpy(sem_name, name);

#ifdef DEBUG
	fprintf(out, "semaphore %s created, start: %d, max: %d\n", sem_name, start, max_msgs);
	fflush(out);
#endif

	hSemaphore = CreateSemaphore(
		NULL,
		start,			// initial count
		max_msgs,	// maximum count
		sem_name	// pointer to semaphore-object name
	);

	if (hSemaphore == NULL)
	{
#ifdef DEBUG
		fprintf(out, "semaphore %s: error: %d\n", sem_name, GetLastError());
		fflush(out);
#endif
    
	}
}


Semaphore::~Semaphore()
{
	if (sem_name)
		delete [] sem_name;

	CloseHandle(hSemaphore);
}

/*
	Signal that there is a new ticket up for grabs,
	use for when done
*/
BOOL Semaphore::Signal()
{
#ifdef DEBUG
	fprintf(out, "semaphore %s, signal\n", sem_name);
	fflush(out);
#endif
	return ReleaseSemaphore(
		hSemaphore,   // handle to the semaphore object
		1,  // amount to add to current count
		NULL   // address of previous count
	);
}

/*
	Wait if there are no "tickets",
	use for when starting
*/
void Semaphore::Wait()
{
#ifdef DEBUG
	fprintf(out, "semaphore %s, wait\n", sem_name);
	fflush(out);
#endif

	WaitForSingleObject(hSemaphore, INFINITE);
}


Mutex::Mutex(LPCSTR name)
{
	mut_name = new char[strlen(name)+1];
	strcpy(mut_name, name);

#ifdef DEBUG
	fprintf(out, "mutex %s created\n", mut_name);
	fflush(out);
#endif

	hMutex = CreateMutex( 
    NULL,                       // no security attributes
    FALSE,                      // initially not owned
    mut_name);  // name of mutex

	if (hMutex == NULL)
	{
#ifdef DEBUG
		fprintf(out, "mutex %s: error: %d\n", mut_name, GetLastError());
		fflush(out);
#endif
    
	}
}

Mutex::~Mutex()
{
	if (mut_name)
		delete [] mut_name;

	CloseHandle(hMutex);
}

/*
	Signal that the mutex can be used again
*/
BOOL Mutex::Signal()
{
	BOOL release;
#ifdef DEBUG
	fprintf(out, "mutex %s, signal\n", mut_name);
	fflush(out);
#endif
	
	release = ReleaseMutex(
		hMutex   // handle to the mutex object
	);

#ifdef DEBUG
	fprintf(out, "mutex %s, signal done\n", mut_name);
	fflush(out);
#endif

	return release;
}

/*
	Take claim of this mutex, or wait
*/
void Mutex::Wait()
{
#ifdef DEBUG
	fprintf(out, "mutex %s, wait\n", mut_name);
	fflush(out);
#endif

	WaitForSingleObject(hMutex, INFINITE);

#ifdef DEBUG
	fprintf(out, "mutex %s, wait done\n", mut_name);
	fflush(out);
#endif

}