/*
  Copyright (C) 2013 Mihai Vasilian

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program. If not, see http://www.gnu.org/licenses/.

  contact: grayasm@gmail.com
*/

//this
#include "test_event.hpp"


//c++
#include <iostream>


//CppUnit
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


//libraries
#include "string.hpp"
#include "exception.hpp"
#include "thread.hpp"
#include "unistd.hpp"
#include "stream.hpp"
#include "vector.hpp"


//###########################BEGIN TEST CLASS ####################################
void test_event::setUp()
{
}

void test_event::tearDown()
{
}

//##########################BEGIN TEST SUITE######################################
void test_event::ctor()
{
	misc::cout << "\n\n\t*******************************************************";
    misc::cout <<   "\n\t* TESTING HEADER: event.hpp                           *";
    misc::cout <<   "\n\t*******************************************************";

	misc::cout << "\n\n\tctor---------------------------------------------------";
	// simple test for calling class constructor
	{
		misc::event ev;
		CPPUNIT_ASSERT( true );
	}
	{
		misc::event* ev = new misc::event();
		delete ev;
		CPPUNIT_ASSERT( true );
	}
	{
		const int EVNO=10;
		misc::event ev[EVNO];
		for(int i=0; i < EVNO; ++i)
			(ev[i]);
		CPPUNIT_ASSERT( true );
	}
	{
		const int EVNO=10;
		misc::event* ev[EVNO];
		for(int i=0; i < EVNO; ++i)
			ev[i] = new misc::event();
		for(int i=0; i < EVNO; ++i)
			delete ev[i];
	}
}

void test_event::dtor()
{
	misc::cout << "\n\n\tdtor---------------------------------------------------";
	// copy of ::ctor() test function
	{
		misc::event ev;
		CPPUNIT_ASSERT( true );
	}
	{
		misc::event* ev = new misc::event();
		delete ev;
		CPPUNIT_ASSERT( true );
	}
	{
		const int EVNO=10;
		misc::event ev[EVNO];
		for(int i=0; i < EVNO; ++i)
			(ev[i]);
		CPPUNIT_ASSERT( true );
	}
	{
		const int EVNO=10;
		misc::event* ev[EVNO];
		for(int i=0; i < EVNO; ++i)
			ev[i] = new misc::event();
		for(int i=0; i < EVNO; ++i)
			delete ev[i];
	}
}

//! Class: A thread waiting for the event to be signaled
class EWaitForEventThread : public misc::thread
{
public:
	EWaitForEventThread(misc::event* ev, int sec)
	: m_ev(ev), m_sec(sec){}
	~EWaitForEventThread() {}
	unsigned long run()
	{
		printf("\n\t\tthread %d: wait event", m_sec);
		CPPUNIT_ASSERT( m_ev->lock() == 0 );
		printf("\n\t\tthread %d: woke up-working", m_sec);
		sleep(m_sec);
		return m_sec;
	}
private:
	misc::event*	m_ev;
	int				m_sec;
};

//! Class: A thread signaling an event.
class ESignalEventThread : public misc::thread
{
public:
	ESignalEventThread(misc::event* ev, int sec)
	: m_ev(ev), m_sec(sec){}
	~ESignalEventThread(){}
	unsigned long run()
	{
		printf("\n\t\tthread %d: wait", m_sec);
		sleep(m_sec);
		printf("\n\t\tthread %d: setevent", m_sec);
		CPPUNIT_ASSERT( m_ev->setevent() == 0 );
		return m_sec;		
	}
private:
	misc::event*	m_ev;
	int				m_sec;
};
void test_event::lock()
{
	misc::cout << "\n\n\tlock---------------------------------------------------";
	misc::cout << "\n\n";
	{
		// 1 thread wait for signal
		misc::event ev;
		EWaitForEventThread t(&ev, 1);
		CPPUNIT_ASSERT( t.resume() == 0 );
		printf("\n\tmain: wait for thread to resume");
		sleep(1);
		CPPUNIT_ASSERT( ev.unlock() == 0 );
		
		CPPUNIT_ASSERT( t.join() == 0 );
		printf("\n\tmain: joined thread");
	}
	misc::cout << "\n\n";
	{
		// more threads waiting for signal
		misc::event ev;
		const int THNO = 600;
		misc::vector<EWaitForEventThread*> t;
		for(size_t i=0; i < THNO; ++i)
			t.push_back(new EWaitForEventThread(&ev, i%6));
		
		for(size_t i=0; i < THNO; ++i)
			CPPUNIT_ASSERT( t[i]->resume() == 0 );
		printf("\n\tmain: resumed %d threads", THNO);

		sleep(5);							// hope all threads are scheduled
		CPPUNIT_ASSERT( ev.unlock() == 0 ); // are all threads listening ?
		
		unsigned long retval = 0;
		for(size_t i=0; i < THNO; ++i)
		{
			CPPUNIT_ASSERT( t[i]->join() == 0 );
			CPPUNIT_ASSERT( t[i]->get_exit_code(&retval) == 0 );
			CPPUNIT_ASSERT( retval == i%6 );
			printf("\n\tmain: joined thread %lu", i);
		}
		for(size_t i=0; i < THNO; ++i)
			delete t[i];
	}
	misc::cout << "\n\n";
	{
		misc::event ev;
		ESignalEventThread t(&ev, 5);
		printf("\n\tmain: wait for signal");
		CPPUNIT_ASSERT( t.resume() == 0 );
		CPPUNIT_ASSERT( ev.lock() == 0 );
		printf("\n\tmain: signaled");
		CPPUNIT_ASSERT( t.join() == 0 );
		printf("\n\tmain: joined thread");
	}
	misc::cout << "\n\n";
	{
		misc::event ev;
		const int THNO=6;
		ESignalEventThread* t[THNO];
		for(int i=0; i < THNO; ++i)
			t[i] = new ESignalEventThread(&ev, i*2+5);
		printf("\n\tmain: resuming %d threads", THNO);
		for(int i=0; i < THNO; ++i)
			CPPUNIT_ASSERT( t[i]->resume() == 0 );
		printf("\n\tmain: waiting for %d signals", THNO);
		for(int i=0; i < THNO; ++i)
		{
			CPPUNIT_ASSERT(ev.lock() == 0);
			printf("\n\tmain: signal");
		}
		for(int i=0; i < THNO; ++i)
		{
			CPPUNIT_ASSERT( t[i]->join() == 0 );
			printf("\n\tmain: joined thread %d", i);
		}
		for(int i=0; i < THNO; ++i)
			delete t[i];
	}
}

//! Class: A thread waiting with timeout for an event;
class ETimedWaitForEventThread : public misc::thread
{
public:
	ETimedWaitForEventThread(misc::event* ev, int sec)
	: m_ev(ev), m_sec(sec), m_signaled(false) {}
	~ETimedWaitForEventThread() {}
	unsigned long run()
	{
		printf("\n\t\tthread %d: timedwait", m_sec);
		if( m_ev->trylock(m_sec*1e3) == 0 )
		{
			printf("\n\t\tthread %d: woke up", m_sec);
			m_signaled = true;
		}
		else
		{
			printf("\n\t\tthread %d: failed to wake up", m_sec);
		}
		return m_sec;
	}
	bool get_signaled() const { return m_signaled; }	
private:
	misc::event*	m_ev;
	int				m_sec;
	bool			m_signaled;
};

void test_event::trylock()
{
	misc::cout << "\n\n\ttrylock------------------------------------------------";	
	misc::cout << "\n\n";
	{
		/*	Thread will wait for an event that is not signaled from main thread
			ever. */
		misc::event ev;
		ETimedWaitForEventThread t(&ev, 0);
		CPPUNIT_ASSERT( t.resume() == 0 );
		CPPUNIT_ASSERT( t.join() == 0 );
		CPPUNIT_ASSERT( t.get_signaled() == false );
	}
	misc::cout << "\n\n";
	{
		/*	Thread will wait for an event that is signaled from main thread. */
		misc::event ev;
		ETimedWaitForEventThread t(&ev, 10);
		CPPUNIT_ASSERT( t.resume() == 0 );
		sleep(2);
		printf("\n\tmain: signaling");
		CPPUNIT_ASSERT( ev.setevent() == 0 );
		CPPUNIT_ASSERT( t.join() == 0 );
		CPPUNIT_ASSERT( t.get_signaled() == true );
		printf("\n\tmain: joined thread");
	}
	misc::cout << "\n\n";
	{
		/* Few threads will wait for an event that will not get signaled. */
		misc::event ev;
		const int THNO=600;
		ETimedWaitForEventThread* t[THNO];
		for(int i=0; i < THNO; ++i)
			t[i] = new ETimedWaitForEventThread(&ev, i%6);
		for(int i=0; i < THNO; ++i)
			CPPUNIT_ASSERT( t[i]->resume() == 0 );
		printf("\n\tmain: threads resumed, no event will be signaled");
		for(int i=0; i < THNO; ++i)
		{
			CPPUNIT_ASSERT( t[i]->join() == 0 );
			CPPUNIT_ASSERT( t[i]->get_signaled() == false );
			printf("\n\tmain: joined thread %d", i);
		}
		for(int i=0; i < THNO; ++i)
			delete t[i];
	}
	misc::cout << "\n\n";
	{
		/* Few threads will wait for an event that will get signaled from main. */
		misc::event ev;
		const int THNO=600;
		ETimedWaitForEventThread* t[THNO];
		for(int i=0; i < THNO; ++i)
			t[i] = new ETimedWaitForEventThread(&ev, 60); // wait reasonable time
		for(int i=0; i < THNO; ++i)
			CPPUNIT_ASSERT( t[i]->resume() == 0 );
		printf("\n\tmain: resumed %d threads", THNO);
		sleep(5);								// hope all are scheduled
		printf("\n\tmain: signaling");
		CPPUNIT_ASSERT( ev.setevent() == 0 );	// are all threads listening ? if not, bad luck.
		for(int i=0; i < THNO; ++i)
		{
			CPPUNIT_ASSERT( t[i]->join() == 0 );
			CPPUNIT_ASSERT( t[i]->get_signaled() == true );
			printf("\n\tmain: joined thread %d", i);
		}
		for(int i=0; i < THNO; ++i)
			delete t[i];
	}
	misc::cout << "\n\n";
	{
		misc::event ev;
		ESignalEventThread t(&ev, 2);
		printf("\n\tmain: wait for event");
		CPPUNIT_ASSERT( t.resume() == 0 );
		CPPUNIT_ASSERT( ev.trylock(120*1e3) == 0 );
		printf("\n\tmain: got the signal");
		CPPUNIT_ASSERT( t.join() == 0 );
		printf("\n\tmain: joined thread");
	}
	misc::cout << "\n\n";
	{
		/*	Starting THNO threads and will get in main a signal from each of them. */
		misc::event ev;
		const int THNO=6;
		ESignalEventThread* t[THNO];
		for(int i=0; i < THNO; ++i)
			t[i] = new ESignalEventThread(&ev, i*2+3);// 2 sec interval
		printf("\n\tmain: resuming %d threads", THNO);
		for(int i=0; i < THNO; ++i)
			CPPUNIT_ASSERT( t[i]->resume() == 0 );
		printf("\n\tmain: waiting for %d signals", THNO);
		for(int i=0; i < THNO; ++i)
		{
			CPPUNIT_ASSERT(ev.trylock(180*1e3) == 0);
			printf("\n\tmain: signal");
		}
		for(int i=0; i < THNO; ++i)
		{
			CPPUNIT_ASSERT( t[i]->join() == 0 );
			printf("\n\tmain: joined thread %d", i);
		}
		for(int i=0; i < THNO; ++i)
			delete t[i];
	}
}

class EWaitForEventThread2 : public misc::thread
{
public:
	EWaitForEventThread2(misc::event* ev, int sec)
	: m_ev(ev), m_sec(sec) {}
	~EWaitForEventThread2() {}
	misc::event* get_event() const { return m_ev; }
	unsigned long run()
	{
		printf("\n\t\tthread %d: waiting", m_sec);
		CPPUNIT_ASSERT( m_ev->lock() == 0 );
		printf("\n\t\tthread %d: signaled, working", m_sec);
		sleep(m_sec);
		return m_sec;
	}
private:
	misc::event*	m_ev;
	int				m_sec;
};

void test_event::unlock()
{
	misc::cout << "\n\n\tunlock-------------------------------------------------";
	misc::cout << "\n\n";
	{
		/* No thread locked on the event. */
		misc::event ev;
		ETimedWaitForEventThread t(&ev, 0);
		printf("\n\tmain: thread resume");
		CPPUNIT_ASSERT( t.resume() == 0 );
		sleep(5);
		CPPUNIT_ASSERT( ev.unlock() == 0 );
		CPPUNIT_ASSERT( t.join() == 0 );
		printf("\n\tmain: joined thread");
		CPPUNIT_ASSERT( t.get_signaled() == false );		
	}
	misc::cout << "\n\n";
	{
		/* 1 thread locked on the event. */
		misc::event ev;
		ETimedWaitForEventThread t(&ev, 10);
		printf("\n\tmain: thread resume");
		CPPUNIT_ASSERT( t.resume() == 0 );
		sleep(5);		
		CPPUNIT_ASSERT( ev.unlock() == 0 );
		CPPUNIT_ASSERT( t.join() == 0 );
		printf("\n\tmain: joined thread");
		CPPUNIT_ASSERT( t.get_signaled() == true );	
	}
	misc::cout << "\n\n";
	{
		const int THNO=600;
		EWaitForEventThread2* t[THNO];
		for(int i=0; i < THNO; ++i)
			t[i] = new EWaitForEventThread2(new misc::event(), 2);
		printf("\n\tmain: starting %d threads", THNO);
		for(int i=0; i < THNO; ++i)
			CPPUNIT_ASSERT( t[i]->resume() == 0 );
		sleep(5);								// hope all are scheduled
		printf("\n\tmain: signaling");
		for(int i=0; i < THNO; ++i)
		{
			misc::event* ev = t[i]->get_event();
			CPPUNIT_ASSERT( ev->unlock() == 0 ); // is the thread listening ? if not, bad luck.
		}
		for(int i=0; i < THNO; ++i)
		{
			CPPUNIT_ASSERT( t[i]->join() == 0 );
			printf("\n\tmain: joined thread %d", i);
		}
		for(int i=0; i < THNO; ++i)
		{
			misc::event* ev = t[i]->get_event();
			delete ev;
			delete t[i];
		}
	}
}
// maybe one of the 2 needed only
void test_event::setevent()
{
	misc::cout << "\n\n\tsetevent-----------------------------------------------";
	CPPUNIT_ASSERT( true ); // all similar with unlock
}


//##########################END  TEST  SUITE######################################