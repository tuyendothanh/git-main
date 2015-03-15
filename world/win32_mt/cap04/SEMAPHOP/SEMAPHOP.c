/*
 * Semaphores get a lot of press because they have the most history in computer
 * science, they are absolutely mired in theory, and professors love asking obscure
 * questions about them.
 * You may be hard pressed to figure out useful examples of where to apply 
 * semaphores off the top of your head, so I'll spoil the surprise and tell you
 * that they are the key ingredient in solving various producer/consumer problems
 * where a buffer is being read and written at the same time.
 * 
 * A semaphore in Win32 may be locked at most n times.
 * Frequently n will end up being the number of threads that can lock a resource
 * but there is no reason why a single thread cannot own all of the locks.
 * A semaphore may replace the need to create a high number of mutexes to handle
 * locking at object level but at the "pool" level.
 * 
 * A semaphore maintains its count assuring that increments and decrements are 
 * handled in an atomic fashion. When a semaphore count gets to zero, whoever was
 * asking for the resource has to wait.
 *
 * A Mutex is often referred to as a BINARY SEMAPHORE. If one thread owns a binary
 * semaphore, then no other thread can acquire ownership.
 * 
 * You do not receive ownership of the semaphore when the lock succeeds. Because
 * several threads can lock a semaphore at the same time, talking about semaphore
 * ownership is not very meaningful. There is such thing as an exclusive lock on a 
 * semaphore.
 *
 * SEMAPHOP.c sample: CreateSemaphore(), OpenSemaphore(), WaitForSingleObject(),
 *                    ReleaseSemaphore(),CloseHandle()
 */


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "MtVerify.h"



#define MAX_SEMAPHORES	3
#define MAX_THREADS		9

DWORD WINAPI ThreadFunc(LPVOID);



int main()
{	
	HANDLE hSemaphore;
	int i;
	DWORD threadId;
	HANDLE hThreads[MAX_THREADS];
	BOOL rc;

	/* Create a *named* semaphore with all locks taken, such that it is
	 * impossible to acquire a lock on it.
	 * It will be accessed by its name, no global handle available.
	 * I will use ReleaseSemaphore() to allow threads enter its GATE.
	 */ 
	MTVERIFY( hSemaphore = CreateSemaphore(
		NULL,
		0,					// no lock available at the moment
		MAX_SEMAPHORES,		// maxim locks possible
		TEXT("SEMAPHOP"))
		);

	/* Create more threads than locks allowed by semaphore to see the 
	 * concept of semaphore GATE locking at work.
	 * Initially all threads will enter a lock state until semaphore
	 * will allow entrance.
	 */ 
	for(i=0; i<MAX_THREADS; ++i)
	{
		hThreads[i] = CreateThread(
			NULL,
			0,
			ThreadFunc,
			(LPVOID)i,
			0,
			&threadId);
		printf("Launched thread %d\n", i);
	}

	// wait before releasing semaphore
	printf("Wait 3 seconds\n");
	Sleep(3000);

	/* Release object and increment count with MAX_SEMAPHORES,
	 * to allow full use of it
	 */
	MTVERIFY( ReleaseSemaphore(hSemaphore, MAX_SEMAPHORES, NULL) );


	/* Wait all threads to finish */
	rc = WaitForMultipleObjects(MAX_THREADS, hThreads, TRUE, INFINITE);
	MTVERIFY( rc >= WAIT_OBJECT_0 && rc < MAX_THREADS );

	/* Clean up threads */
	for(i=0; i<MAX_THREADS; ++i)
		MTVERIFY( CloseHandle(hThreads[i]) );

	/* Clean up semaphore object */
	MTVERIFY( CloseHandle(hSemaphore) );

	printf("All threads + semaphore clean\n");

	return EXIT_SUCCESS;
}


DWORD WINAPI ThreadFunc(LPVOID n)
{
	DWORD rc;
	HANDLE semaphoreH;

	/* Open a handle to a named semaphore object */
	MTVERIFY( semaphoreH = OpenSemaphore(
								SEMAPHORE_ALL_ACCESS, 
								FALSE, 
								TEXT("SEMAPHOP"))
		);

	/* Ask for permission to enter the SEMAPHORE GATE */
	rc = WaitForSingleObject(semaphoreH, INFINITE);
	MTVERIFY( rc == WAIT_OBJECT_0 );

	printf("Semaphore allowed thread %d\n", n);
	
	srand( GetTickCount() );
	Sleep((rand()%10)*800+500);

	/* Release object and increment count with 1 */
	MTVERIFY( ReleaseSemaphore(semaphoreH, 1, NULL) );
	/* Close handle we got via OpenSemaphore */
	MTVERIFY( CloseHandle(semaphoreH) );

	printf("Thread %d idle\n", n);
	return ((DWORD) n);
}