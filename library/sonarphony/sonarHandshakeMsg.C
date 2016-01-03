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

#include "sonarHandshakeMsg.H"
using namespace sonarphony;

using namespace std;


///////////////////////////////////////////////////////////////////////////
namespace
{

enum
{
	O_SERIAL_NUMBER                 = 16,   ///< Offset to serial number
};

/// \brief Size of the serial number in bytes
unsigned const SERIAL_NUMBER_SIZE       = 10;
}


///////////////////////////////////////////////////////////////////////////
sonarHandshakeMsg_t::~sonarHandshakeMsg_t ()
{
}

sonarHandshakeMsg_t::sonarHandshakeMsg_t (QByteArray const &buffer_) :
	sonarMsg_t (buffer_)
{
	Q_ASSERT (type () == T_HANDSHAKE);
}

sonarHandshakeMsg_t::sonarHandshakeMsg_t (sonarMsg_t const &msg_) :
	sonarMsg_t (msg_)
{
	Q_ASSERT (type () == T_HANDSHAKE);
}

string sonarHandshakeMsg_t::serialNumber () const
{
	Q_ASSERT (m_buffer.size () >=
	          static_cast<int> (O_SERIAL_NUMBER + SERIAL_NUMBER_SIZE));

	string sn;

	for (unsigned i = 0; i < SERIAL_NUMBER_SIZE; ++i)
		sn.push_back (m_buffer[i + O_SERIAL_NUMBER]);

	return sn;
}

