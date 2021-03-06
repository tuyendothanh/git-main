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


#include "BAR.hpp"

namespace fx
{
    BAR::BAR()
    {
        Init();
    }

    BAR::BAR(const stl::string& instrument, int period, Timeframe sec)
    {
        if (period < 2)
            throw stl::exception("BAR period must be greater than 1");

        Init();

        m_instrument = instrument;
        m_period = period;
        m_timeframe = sec;

        //----------------------
        m_lastOHLC.SetInstrument(instrument.c_str());
        m_lastOHLC.SetTimeframe(""); // not used for now
    }

    BAR::~BAR()
    {
    }

    BAR::BAR(const BAR& tc)
    {
        Init();

        *this = tc;
    }

    BAR& BAR::operator=(const BAR& tc)
    {
        if (this != &tc)
        {
            m_instrument = tc.m_instrument;
            m_period = tc.m_period;
            m_timeframe = tc.m_timeframe;
            m_reftime = tc.m_reftime;
            m_lastOHLC = tc.m_lastOHLC;
            m_priceList = tc.m_priceList;
        }
        return *this;
    }

    const stl::string& BAR::GetInstrument() const
    {
        return m_instrument;
    }

    int BAR::GetPeriod() const
    {
        return m_period;
    }

    BAR::Timeframe BAR::GetTimeframe() const
    {
        return m_timeframe;
    }

    bool BAR::IsValid() const
    {
        return (m_period > 1 && m_period == m_priceList.size());
    }


    void BAR::Update(const fx::Offer& offer)
    {
        if (m_instrument != offer.GetInstrument())
            throw stl::exception("BAR offer is invalid");

        // begin at next timeframe normalized up to daily candle
        if (m_reftime.totime_t() == 0)
        {
            m_reftime = offer.GetTime();

            if (m_timeframe >= sys::time::minSEC)
                m_reftime -= m_reftime.sec_();
            if (m_timeframe >= sys::time::hourSEC)
                m_reftime -= (m_reftime.min_() * sys::time::minSEC);
            if (m_timeframe >= sys::time::daySEC)
                m_reftime -= (m_reftime.hour_() * sys::time::hourSEC);

            // if offer is inside the bar, then move to next timeframe
            if (m_reftime != offer.GetTime())
                m_reftime += m_timeframe;
        }

        const sys::time& currtime = offer.GetTime();

        // wait for the reference time to begin with
        if (currtime < m_reftime)
            return;

        sys::time nextt = m_reftime + m_timeframe;
        if (currtime < nextt)
        {
            double bid = offer.GetBid();
            double ask = offer.GetAsk();

            if (m_lastOHLC.GetBidOpen() == 0) // uninitialized
            {
                m_lastOHLC.SetBidOpen(bid);
                m_lastOHLC.SetBidHigh(bid);
                m_lastOHLC.SetBidLow(bid);
                m_lastOHLC.SetBidClose(bid);

                m_lastOHLC.SetAskOpen(ask);
                m_lastOHLC.SetAskHigh(ask);
                m_lastOHLC.SetAskLow(ask);
                m_lastOHLC.SetAskClose(ask);
            }
            else
            {
                m_lastOHLC.SetBidClose(bid);    // can close anytime soon
                m_lastOHLC.SetAskClose(ask);

                if (bid > m_lastOHLC.GetBidHigh())
                {
                    m_lastOHLC.SetBidHigh(bid);
                    m_lastOHLC.SetAskHigh(ask);
                }
                else if (bid < m_lastOHLC.GetBidLow())
                {
                    m_lastOHLC.SetBidLow(bid);
                    m_lastOHLC.SetAskLow(ask);
                }
            }
        }
        else if (currtime >= nextt)
        {
            // handle the list
            m_priceList.push_back(m_lastOHLC);

            // keep period constant
            if (m_priceList.size() > m_period)
                m_priceList.pop_front();

            // new candle has just started
            double bid = offer.GetBid();
            double ask = offer.GetAsk();

            m_lastOHLC.SetBidOpen(bid);
            m_lastOHLC.SetBidHigh(bid);
            m_lastOHLC.SetBidLow(bid);
            m_lastOHLC.SetBidClose(bid);

            m_lastOHLC.SetAskOpen(ask);
            m_lastOHLC.SetAskHigh(ask);
            m_lastOHLC.SetAskLow(ask);
            m_lastOHLC.SetAskClose(ask);

            // current candle open (including weekend gap)
            while (currtime >= m_reftime + m_timeframe)
                m_reftime += m_timeframe;
        }
    }

    const sys::time& BAR::GetRefTime() const
    {
        return m_reftime;
    }

    bool BAR::IsNew(const fx::Offer& offer) const
    {
        if (m_reftime.totime_t() == 0)  // not initialized
            return false;

        const sys::time& currtime = offer.GetTime();    
        sys::time nextt = m_reftime + m_timeframe;
        return (currtime >= nextt);
    }

    const fx::OHLCPrice& BAR::GetOHLC() const
    {
        return m_lastOHLC;
    }

    const BAR::OHLCPriceList& BAR::GetOHLCList() const
    {
        return m_priceList;
    }

    void BAR::Init()
    {
        m_instrument = "";
        m_period = -1;
        m_timeframe = 0;
        // m_reftime - default
        m_lastOHLC = fx::OHLCPrice("", "", sys::time(),
                                    0, 0, 0, 0,
                                    0, 0, 0, 0,
                                    0);
        // m_priceList - default
    }

} // namespace