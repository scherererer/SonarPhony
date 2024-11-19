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

#include "masterHandshake.hh"
#include "msgPrivate.hh"
using namespace sonarphony;


///////////////////////////////////////////////////////////////////////////
namespace
{

/// \brief Total size of the message
unsigned const MESSAGE_SIZE     = 71;

/// \brief Magic header data. FX
unsigned char const HEADER[] = {
	0x46, 0x58, 0x15, 0x00
};

/// \brief Magic header data. FV
unsigned char const HEADER_PAUSE[] = {
	0x46, 0x56, 0x15, 0x00
};

/// \brief Magic "middle" data after header.
unsigned char const MAGIC_MIDDLE[15] = { 0x00 };

/// \brief Magic footer
unsigned char const FOOTER[9] = { 0x00 };

/// \brief Magic header data. FM
unsigned char const HEADER_MASTER[] = {
	0x46, 0x4d, 0x15, 0x00
};

/// \brief Magic header data. FM
unsigned char const MASTER_CODE[] = {
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, // Default 12345678
};

/// \brief Session Code
unsigned char const SESSION[] = {
	0x00, 0x17, 0x31, 0x98, 0x71, 0x81, 0x48
};

unsigned char const MASTER_FOOTER[] = {
	0x04, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, // Default 12345678 Repeated?
};

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

QByteArray const &masterHandshakeBuilder_t::buildPause ()
{
	m_buffer.clear ();

	// Add magic header
	m_buffer.append (_U2S (HEADER_PAUSE), sizeof (HEADER_PAUSE));

	// Don't know what any of this is yet
	m_buffer.append (_U2S (MAGIC_MIDDLE), sizeof (MAGIC_MIDDLE));

	// Checksum
	m_buffer.append (calculateChecksum (m_buffer));

	// Magical message footer
	m_buffer.append (_U2S (FOOTER), sizeof (FOOTER));

	return m_buffer;
}

QByteArray const &masterHandshakeBuilder_t::buildMaster ()
{
	m_buffer.clear ();

	// Add magic header
	m_buffer.append (_U2S (HEADER_MASTER), sizeof (HEADER_MASTER));

	// Master code
	m_buffer.append (_U2S (MASTER_CODE), sizeof (MASTER_CODE));

	// Desired session ID
	m_buffer.append (_U2S (SESSION), sizeof (SESSION));

	// Checksum
	m_buffer.append (calculateChecksum (m_buffer));

	// Don't know what any of this is yet
	m_buffer.append (_U2S (MASTER_FOOTER), sizeof (MASTER_FOOTER));

	return m_buffer;
}
