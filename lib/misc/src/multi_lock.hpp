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


#ifndef __multi_lock_hpp__
#define __multi_lock_hpp__


#include "sync_base.hpp"


namespace misc
{	

	class multi_lock
	{
	public:
		multi_lock(sync_base* objects[], unsigned long objcount);
		
		~multi_lock();

		int lock();

		int trylock(unsigned long milliseconds, bool wait_for_all);

		int unlock();

	private:
		//! non-copyable
		multi_lock(const multi_lock&);
		multi_lock& operator=(const multi_lock&);
		
	protected:
		sync_base**		m_objects;
		unsigned long		m_count;
	};

}  // namespace


#endif // __multi_lock_hpp__
