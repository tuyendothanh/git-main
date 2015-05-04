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


#ifndef __event_hpp__
#define __event_hpp__



#include "sync_base.hpp"

namespace misc
{	
	class event : public sync_base
	{
	public:

		event();
		~event();
		int lock();
		int trylock(unsigned long milliseconds = INFINITE);		
		int unlock();
		int setevent();
		int pulseevent();
		int resetevent();

	private:		
		event(const event& tc);
		event& operator=(const event& tc);
		
	private:
#ifdef _WIN32
#else
		pthread_mutex_t		m_mtx;
		pthread_cond_t		m_cond;
#endif	
	};

}  // namespace



#endif // __event_hpp__


