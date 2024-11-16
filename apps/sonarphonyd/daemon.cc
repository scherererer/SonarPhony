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

#include "daemon.hh"

#include "sonarphony/pingMsg.hh"
using namespace sonarphony;

#include <iostream>
using namespace std;


///////////////////////////////////////////////////////////////////////////
namespace
{

/// \brief Calculate a NMEA0183 checksum
inline int nmea0183checksum (char *s_)
{
	int cksum = 0;

	while (*s_)
		cksum ^= *s_++;

	return cksum;
}

}


///////////////////////////////////////////////////////////////////////////
daemon_t::~daemon_t ()
{
}

daemon_t::daemon_t ()
	: m_connection(this)
	, m_udpPort(0)
	, m_socket(this)
{
	connect (&m_connection, SIGNAL (ping (sonarphony::pingMsg_t const &)),
	         SLOT (handlePing (sonarphony::pingMsg_t const &)));
}

void daemon_t::initialize (int const udpPort_)
{
	m_connection.start ();
	m_udpPort = udpPort_;

	if (udpPort_ == 0)
		return;
}

void daemon_t::handlePing (pingMsg_t const &ping_)
{
	// NMEA0183 'specification' taken from:
	// http://www.nmea.de/nmea0183datensaetze.html
	// https://en.wikipedia.org/wiki/NMEA_0183

	static char buffer[1024];

	int chars = snprintf (buffer, sizeof (buffer),
	                      "$SDDPT,%.1f,0.0*", ping_.depth ());

	if (chars <= 0)
		return;

	chars = snprintf (buffer + chars, sizeof (buffer),
	                  "%02X\r\n", nmea0183checksum (buffer));

	if (chars > 0)
		send(QByteArray::fromRawData(buffer, chars));

	// MTW - Water Temperature
	//
	//	1   2 3
	//	|   | |
	// $--MTW,x.x,C*hh<CR><LF>
	//
	// Field Number:
	//  1) Degrees
	//  2) Unit of Measurement, Celcius
	//  3) Checksum
	chars = snprintf (buffer, sizeof (buffer),
	                  "$SDMTW,%.1f,C*", ping_.temperature ());

	if (chars <= 0)
		return;

	chars = snprintf (buffer + chars, sizeof (buffer),
	                  "%02X\r\n", nmea0183checksum (buffer));

	if (chars > 0)
		send(QByteArray::fromRawData(buffer, chars));
}

void daemon_t::send(QByteArray const &buffer_)
{
	if (m_udpPort == 0)
	{
		cout << buffer_.constData();
		return;
	}

	m_socket.writeDatagram(buffer_, QHostAddress::Broadcast, m_udpPort);
}
