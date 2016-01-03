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

#include <QTimer>
#include <QUdpSocket>

namespace sonarphony
{

class pingMsg_t;

/// \brief A connection to the sonar device
class sonarConnection_t : public QObject
{
	Q_OBJECT
public:
	~sonarConnection_t ();
	/// \param parent_ Parent object for the connection
	explicit sonarConnection_t (QObject *parent_ = 0);

	/// \brief Set the range
	/// \param min_ Minimum range (upper limit) [0, ?] feet
	/// \param max_ Maximum range (lower limit) [0, 240] feet
	///
	/// The maximum range for the T-Pod is 120 feet, while the T-Box is
	/// rated to 240 feet, although this has not been tested. Setting both
	/// values to 0 will enable auto-ranging.
	void setRange (double min_, double max_);

	/// \brief Get the serial number for the sonar on this connection
	std::string serialNumber () const;

signals:
	/// \brief New data is ready
	/// \param data_ Available data
	/// \note This is only used for debugging, not sure I want to keep this
	void newData (QByteArray const &data_);

	/// \brief A new ping message has been returned
	/// \param ping_ The ping message
	void ping (sonarphony::pingMsg_t const &ping_);

	/// \brief Signaled when the serial number on the device changes or is
	/// received for the first time
	void serialNumberChanged ();

public slots:
	/// \brief Start polling the device
	void start ();
	/// \brief Stop polling the device
	/// \note The device will continue to send pings until it times out
	void stop ();

private slots:
	/// \brief Send an actual query to the device
	void query ();
	/// \brief Handle a message from the device
	void handleDatagrams ();

private:
	class private_t;
	private_t * const m_d;          ///< Private data
};

}
