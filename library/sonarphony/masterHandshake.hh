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

#include <QByteArray>


namespace sonarphony
{

/// \brief Constructs a "master" handshake message
class masterHandshakeBuilder_t
{
public:
	~masterHandshakeBuilder_t ();
	masterHandshakeBuilder_t ();

	/// \brief Build the message
	QByteArray const &build ();

	/// \brief Build the pause message
	QByteArray const &buildPause ();

	/// \brief Build the master message
	QByteArray const &buildMaster ();

private:
	QByteArray m_buffer;    ///< Message buffer
};

}
