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

#include "masterCommand.hh"
#include "msgPrivate.hh"
using namespace sonarphony;


///////////////////////////////////////////////////////////////////////////
namespace
{

/// \brief Total message size
unsigned const MESSAGE_SIZE     = 71;

/// \brief Magical header data
unsigned char const HEADER[] = {
	0x46, 0x43, 0x15, 0x00, 0x2c, 0x01
};

/// \brief Magical middle data
unsigned char const MAGIC_MIDDLE[] = {
	0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
};

//02: second
//04: T-POD, 125 kHz
//08: first

/// \brief Session Code
unsigned char const SESSION[] = {
	0x00, 0x17, 0x31, 0x98, 0x71, 0x81, 0x48
};

/// \brief Magical footer
unsigned char const FOOTER[] = {
	0x00, 0x00
};

}


///////////////////////////////////////////////////////////////////////////
masterCommandBuilder_t::~masterCommandBuilder_t ()
{
}

masterCommandBuilder_t::masterCommandBuilder_t () :
	m_buffer (),
	m_minRange (0),
	m_maxRange (0),
    m_frequency (4)
{
	m_buffer.reserve (MESSAGE_SIZE);
}

void masterCommandBuilder_t::setRange (unsigned min_, unsigned max_)
{
	Q_ASSERT (max_ <= 240);
	Q_ASSERT (min_ < max_ || min_ == 0);

	m_minRange = min_;
	m_maxRange = max_;
}

void masterCommandBuilder_t::setFrequency (frequency_t freq_)
{
    switch(freq_)
    {
    case F_80:  m_frequency = 2; break;
    case F_125: m_frequency = 4; break;
    case F_200: m_frequency = 8; break;
    }
}

QByteArray const &masterCommandBuilder_t::build ()
{
	m_buffer.clear ();

	// Add magic header
	m_buffer.append (_U2S (HEADER), sizeof (HEADER));

	// Encode minimum range
	m_buffer.append (static_cast<char> (m_minRange));
	// Encode zeros between. It is possible this is the upper byte of the
	// max range, however since none of the available hardware is rated for
	// more than 240 ft (0xF0, 1 byte's worth), I don't know if this is the
	// case and so haven't been able to verify this.
	m_buffer.append (char (0));
	// Encode maximum range
	m_buffer.append (static_cast<char> (m_maxRange));

	// Don't know what any of this is yet
    // 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
	m_buffer.append (char (0x00));
	m_buffer.append (char (0x00));
	m_buffer.append (char (0x01));
	m_buffer.append (char (0x00));
	m_buffer.append (static_cast<char> (m_frequency));
	m_buffer.append (char (0x00));
	m_buffer.append (char (0x00));
	m_buffer.append (char (0x00));
	m_buffer.append (char (0x00));
	m_buffer.append (char (0x00));

	// Checksum. Why that's needed inside of a UDP packet I'll never know
	m_buffer.append (calculateChecksum (m_buffer));

	// Session
	m_buffer.append (_U2S (SESSION), sizeof (SESSION));
	// Stop bytes
	m_buffer.append (_U2S (FOOTER), sizeof (FOOTER));

	return m_buffer;
}

