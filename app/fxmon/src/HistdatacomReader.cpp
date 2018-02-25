/*
	Copyright (C) 2018 Mihai Vasilian

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

#include "HistdatacomReader.hpp"
#include "unistd.hpp"
#include "filename.hpp"
#include "stream.hpp"


HistdatacomReader::HistdatacomReader(const misc::string& instrument)
{
	m_instrument = instrument;

	
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201701.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201701_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201702.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201702_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201703.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201703_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201704.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201704_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201705.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201705_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201706.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201706_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201707.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201707_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201708.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201708_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201709.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201709_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201710.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201710_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201711.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201711_2.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201712.csv");
	m_offerFileVec.push_back("d:\\GitHub\\histdatacom-history\\DAT_ASCII_EURUSD_T_201712_2.csv");
	

	m_ofvPos = 0;
	// m_offersVec; - empty
	m_ovPos = 0;
}

HistdatacomReader::~HistdatacomReader()
{
}

bool HistdatacomReader::GetOffer(fx::Offer& offer)
{
	if (m_ovPos < m_offersVec.size())
	{
		offer = m_offersVec[m_ovPos++];
		if (offer.GetPrecision() == 0 || offer.GetPointSize() == 0 ||
			offer.GetBid() == 0.0 || offer.GetAsk() == 0.0)
		{
			// invalid quote
			return GetOffer(offer);
		}

		return true;
	}

	if (m_ofvPos < m_offerFileVec.size())
	{
		m_offersVec.resize(0);
		m_ovPos = 0;
		ParseFile(m_offerFileVec[m_ofvPos++], m_offersVec);
		return GetOffer(offer);
	}

	return false;
}


void HistdatacomReader::ParseFile(const misc::string& filePath,
	misc::vector<fx::Offer>& result)
{
	misc::filename offerFile(filePath);
	if (!offerFile.access(F_OK))
		return; // error

	FILE* pf = fopen(offerFile.get_path().c_str(), "r");
	if (pf == NULL)
		return; // error

	const int bufsz = 1000000;
	char buf[bufsz];
	int bufmax = 0;
	int bufpos = 0;

	// assume EUR/USD for now
	fx::Offer offer(
		"0",
		"EUR/USD",
		5,		// precision
		0.0001, // pointSize
		misc::time(),
		0,		// bid
		0,		// ask
		0,		// vol
		true);	// trading is open

	misc::string fline;
	char c;

	int year = 0;
	int month = 0;
	int day = 0;


	while (true) //(c = fgetc(pf)) != EOF)
	{
		if (bufpos < bufmax) {
			c = buf[bufpos++];
		}
		else if (!feof(pf)) {
			bufmax = fread(buf, sizeof(char), bufsz, pf);
			bufpos = 0;
			continue;
		}
		else {
			// end of stream
			break;
		}


		if (c == '\n')
		{
			char* pch;
			char str[1000];
			strcpy(str, fline.c_str());
			misc::string s1;
			int i = 0;

			pch = strtok(str, " ,");
			while (pch != NULL)
			{
				i++;
				// 20171201 000000043,1.191100,1.191140,0
				// 20171201 002359120,1.191100,1.191140,0
				if (i == 1)
				{
					s1 = misc::string(pch);
					int fulldate = 0;
					misc::to_value(s1, fulldate);
					year = fulldate / 10000;
					month = (fulldate - year * 10000) / 100;
					day = fulldate - year * 10000 - month * 100;
				}
				else if (i == 2)
				{
					s1 = misc::string(pch);
					int fulltime = 0;
					misc::to_value(s1, fulltime);
					int hour = fulltime / 10000000;
					int min = fulltime / 100000;
					if (min >= 100)
						min -= hour * 100;
					int milsec = fulltime - hour * 10000000 - min * 100000;
					int sec = milsec / 1000;

					misc::time::Month mon = (misc::time::Month)(misc::time::JAN + month - 1);

					misc::time otime(year, mon, day, hour, min, sec);
					// time is EST (Eastern Time without Daylight Saving)
					// because I calculate with UTC makes sense to covert now
					otime += 5 * misc::time::hourSEC;

					offer.SetTime(otime);
				}
				else if (i == 3)
				{
					s1 = misc::string(pch);
					double bid = 0;
					misc::to_value(s1, bid);
					offer.SetBid(bid);					
				}
				else if (i == 4)
				{
					s1 = misc::string(pch);
					double ask = 0;
					misc::to_value(s1, ask);
					offer.SetAsk(ask);
				}
				else if (i == 5)
				{
					s1 = misc::string(pch);
					double vol = 0;
					misc::to_value(s1, vol);
					offer.SetVolume(vol);
				}

				pch = strtok(NULL, ",=");
			} // while(pch)

			if (m_instrument == offer.GetInstrument())
			{
				result.push_back(offer);

#ifdef DEBUG
				if (result.size() % 1000 == 0)
					misc::cout << "fetched " << result.size() << std::endl;
#endif
			}

			fline.resize(0);
			continue;
		} // c == '\n'

		fline.append(1, c);
	}

	fclose(pf);
}