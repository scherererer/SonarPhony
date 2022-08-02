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

#include "sonarphony/sonarMsg.H"

#include <QByteArray>

#include <string>


namespace sonarphony
{

/// \brief Handshake message from the sonar unit
class sonarHandshakeMsg_t : public sonarMsg_t
{
public:
	~sonarHandshakeMsg_t ();
	/// \param buffer_ Buffer to construct the message from
	explicit sonarHandshakeMsg_t (QByteArray const &buffer_);
	/// \brief Copy constructor
	/// \param msg_ Message to copy
	sonarHandshakeMsg_t (sonarMsg_t const &msg_);
	sonarHandshakeMsg_t& operator= (const sonarHandshakeMsg_t&) = default;

	/// \brief Get the serial number of the device
	std::string serialNumber () const;
};

}
