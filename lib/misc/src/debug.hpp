/*
  Copyright (C) 2014 Mihai Vasilian

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

#ifndef __debug_hpp__
#define __debug_hpp__


#ifdef _WIN32
#	define DEBUG_VIEW(text)	\
		std::stringstream ss;	\
		ss << __FUNCTION__ << " line: " << __LINE__ << " [" << text << "]"; \
		OutputDebugString(ss.rdbuf()->str().c_str());
#else
#	define DEBUG_VIEW(text)	\
		std::cout << "\n" << __FUNCTION__ << " line: " << __LINE__ << " [" << a << "]";
#endif


#endif //__debug_hpp__
