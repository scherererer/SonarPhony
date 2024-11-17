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

#pragma once

#include "sonarphony/sonarConnection.hh"
#include "sonarphony/logger.hh"

#include <QByteArray>
#include <QObject>
#include <QUdpSocket>


class daemon_t : public QObject
{
	Q_OBJECT
public:
	~daemon_t();
	daemon_t();

	/// \brief Initialize the daemon
	void initialize(int udpPort_);

	void rawLog(std::string directory_);

private slots:
	/// \brief Handle a ping message
	/// \param ping_ Ping message to handle
	void handlePing(sonarphony::pingMsg_t const &ping_);

private:
	void send(QByteArray const &buffer_);

	/// \brief Connection to the sonar unit
	sonarphony::sonarConnection_t m_connection;
	/// \brief Port for broadcast traffic
	int m_udpPort;
	/// \brief Socket for UDP messages
	QUdpSocket m_socket;

	sonarphony::rawLogger_t m_rawLogger;
};
