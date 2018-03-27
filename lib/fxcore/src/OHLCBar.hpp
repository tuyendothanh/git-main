/*
	Copyright (C) 2018 Mihai Vasilian

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software Foundation,
	Inc. 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

	contact: grayasm@gmail.com
*/

#ifndef __OHLCBar_hpp__
#define __OHLCBar_hpp__

#include "string.hpp"
#include "time.hpp"
#include "Offer.hpp"
#include "OHLCPrice.hpp"


namespace fx
{
	class OHLCBar
	{
	public:
		typedef time_t	Timeframe;

		OHLCBar();
		OHLCBar(const misc::string& instrument, Timeframe sec);
		~OHLCBar();
		OHLCBar(const OHLCBar& tc);
		OHLCBar& operator=(const OHLCBar& tc);
		
		void Update(const fx::Offer& offer);

		const fx::OHLCPrice& GetOHLC() const;
		

	private:
		void Init();

	private:
		misc::string		m_instrument;
		time_t				m_timeframe;
		// ----------------------------
		misc::time			m_reftime;
		fx::OHLCPrice		m_lastOHLC;
	};
} // namespace

#endif // __OHLCBar_hpp__
