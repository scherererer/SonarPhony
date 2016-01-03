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

#include "masterHandshake.H"
#include "msgPrivate.H"
using namespace sonarphony;


///////////////////////////////////////////////////////////////////////////
namespace
{

/// \brief Total size of the message
unsigned const MESSAGE_SIZE     = 71;

/// \brief Magic header data.
unsigned char const HEADER[] = {
	0x46, 0x58, 0x15, 0x00, 0x00, 0x00
};

/// \brief Magic "middle" data after header.
unsigned char const MAGIC_MIDDLE[13] = { 0x00 };
/// \brief Magic footer
unsigned char const FOOTER[9] = { 0x00 };

}


///////////////////////////////////////////////////////////////////////////
masterHandshakeBuilder_t::~masterHandshakeBuilder_t ()
{
}

masterHandshakeBuilder_t::masterHandshakeBuilder_t () :
	m_buffer ()
{
	m_buffer.reserve (MESSAGE_SIZE);
}

QByteArray const &masterHandshakeBuilder_t::build ()
{
	m_buffer.clear ();

	// Add magic header
	m_buffer.append (_U2S (HEADER), sizeof (HEADER));

	// Don't know what any of this is yet
	m_buffer.append (_U2S (MAGIC_MIDDLE), sizeof (MAGIC_MIDDLE));

	// Checksum
	m_buffer.append (calculateChecksum (m_buffer));

	// Magical message footer
	m_buffer.append (_U2S (FOOTER), sizeof (FOOTER));

	return m_buffer;
}
