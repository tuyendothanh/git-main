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

#ifndef __HABAR_hpp__
#define __HABAR_hpp__

#include "BAR.hpp"

namespace fx
{
	class HABAR : protected BAR
	{
	public:
		HABAR();
		HABAR(const misc::string& instrument, int period, Timeframe sec);
		~HABAR();
		HABAR(const HABAR& tc);
		HABAR& operator=(const HABAR& tc);

		// --- virtual table ---
		const misc::string& GetInstrument() const override;
		int GetPeriod() const override;
		Timeframe GetTimeframe() const override;
		bool IsValid() const override;
		void Update(const fx::Offer& offer) override;
		const misc::time& GetRefTime() const override;
		// --- end of virtual table ---

		bool IsNew(const misc::time& currtime) const;
		const fx::OHLCPrice& GetHA() const;

	private:
		void Init();

	private:
		fx::OHLCPrice		m_lastHA;
		OHLCPriceList		m_haList;
	};
} // namespace

#endif // __HABAR_hpp__