// This file is part of SonarPhony.
//
// SonarPhony is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SonarPhony is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SonarPhony.  If not, see <http://www.gnu.org/licenses/>.

#include "sonarphony/pingMsg.hh"
using namespace sonarphony;

#include <QtEndian>

#include <algorithm>


///////////////////////////////////////////////////////////////////////////
namespace
{

template <typename T>
inline T letoh (char const *buffer_)
{
	return qFromLittleEndian<T> (reinterpret_cast<uchar const *> (buffer_));
}

inline double clip (double value_, double min_, double max_)
{
	return std::max (min_, std::min (value_, max_));
}

enum
{
	// FF00FF00
	// REDYFCJ
	// 4A   [12]    < const
	// 01   [13]    < const
	// 2C   [14]    < const
	// 01   [15]    < const
	O_MIN_RANGE     = 16, // 2 bytes LE
	// 00   [17]  upper bytes of min range
	O_MAX_RANGE     = 18, // 2 bytes LE
	// 00   [20]    < const upper bytes of max range
	// 01   [21]    < const
	// 00   [22]    < const
	O_DEPTH         = 23, // Depth
	// 00   [24]    < const upper bytes of depth
	O_DEPTH_FRAC    = 25, // [0,9] fractional portion of depth
	O_TEMPERATURE   = 26, // 1 byte (possibly 2?)
	// 00   [27]    < const < temp upper? -- unlikely
	// 14   [28]    < const (20 / 276)
	// 01   [29]    < const (1  / 1025)
	O_BATTERY       = 30, // << Whole part of battery voltage
	O_BATTERY_FRAC  = 31, // << Fractional part of battery voltage [100-0]
	O_PING_FREQ     = 32, // 04   [32]    < Ping Type 2/4/8
	// A2.B5.78.EF.F5 -- 5 byte footer
	O_PING_BEGIN    = 38,   //< const ?
	// [39]         < const?
	// [40]         < 99% data here
	// [41]         < Data definitely at least here
};

/// \brief Highest voltage I have ever whitnessed
double const BATTERY_MAX_VOLTAGE = 4.2;
/// \brief Lowest voltage I have ever whitnessed
double const BATTERY_MIN_VOLTAGE = 2.4;

}


///////////////////////////////////////////////////////////////////////////
pingMsg_t::~pingMsg_t ()
{
}

pingMsg_t::pingMsg_t () :
	sonarMsg_t ()
{
}

pingMsg_t::pingMsg_t (QByteArray const &buffer_) :
	sonarMsg_t (buffer_)
{
	Q_ASSERT (type () == T_PING);
}

pingMsg_t::pingMsg_t (sonarMsg_t const &msg_) :
	sonarMsg_t (msg_)
{
	Q_ASSERT (type () == T_PING);
}

pingMsg_t::pingMsg_t (pingMsg_t const &msg_) :
	sonarMsg_t (msg_)
{
	Q_ASSERT (type () == T_PING);
}

unsigned pingMsg_t::minRange () const
{
	Q_ASSERT (m_buffer.size () > O_MIN_RANGE);

	return letoh<quint16> (m_buffer.constData () + O_MIN_RANGE);
}

unsigned pingMsg_t::maxRange () const
{
	Q_ASSERT (m_buffer.size () > O_MAX_RANGE);

	return letoh<quint16> (m_buffer.constData () + O_MAX_RANGE);
}

double pingMsg_t::depth () const
{
	Q_ASSERT (m_buffer.size () > O_DEPTH);

	return letoh<quint16> (m_buffer.constData () + O_DEPTH) +
	       (m_buffer[O_DEPTH_FRAC] * 0.1);
}

double pingMsg_t::temperature () const
{
	Q_ASSERT (m_buffer.size () > O_TEMPERATURE);

	double const rawTemp = static_cast<unsigned> (m_buffer[O_TEMPERATURE]);

	// There is a slight bias between the raw value and the output
	// temperature. I conjecture that the reason for this is that sea
	// water's freezing point is around -2 C, thus it would make more sense
	// for that to be the lower temperature bound.
	return rawTemp - 2;
}

double pingMsg_t::batteryVoltage () const
{
	Q_ASSERT (m_buffer.size () > O_BATTERY_FRAC);

	return static_cast<double> (m_buffer[O_BATTERY]) +
	       static_cast<double> (m_buffer[O_BATTERY_FRAC]) * .01;
}

unsigned pingMsg_t::batteryLevel () const
{
	double const range = BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE;

	return clip (((batteryVoltage () - BATTERY_MIN_VOLTAGE) / range) * 100,
	             0, 100);
}

double pingMsg_t::frequency () const
{
    unsigned frequency = m_buffer[O_PING_FREQ];
    //02: second
    //04: T-POD, 125 kHz, 30 degree
    //08: first
    //200/83 Dual Beam 20/40

    switch (frequency)
    {
    case 2:
        return  80000;
    case 4:
        return 125000;
    case 8:
        return 200000;
    }

    qCritical("Unknown ping frequency option %02X", frequency);
    return 0;
}

double pingMsg_t::beamWidth () const
{
    unsigned frequency = m_buffer[O_PING_FREQ];
    //02: second
    //04: T-POD, 125 kHz, 30 degree
    //08: first
    //200/83 Dual Beam 20/40

    switch (frequency)
    {
    case 2:
        return 40;
    case 4:
        return 30;
    case 8:
        return 20;
    }

    qCritical("Unknown ping frequency option %02X", frequency);
    return 0;
}

char const *pingMsg_t::pingData () const
{
	Q_ASSERT (m_buffer.size () > O_PING_BEGIN);

	return m_buffer.constData () + O_PING_BEGIN;
}

unsigned pingMsg_t::pingSize () const
{
	return m_buffer.size () - O_PING_BEGIN;
}

