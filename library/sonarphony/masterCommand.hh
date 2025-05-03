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

#include "sonarphony/global.hh"
#include "sonarphony/sonarConnection.hh"

#include <QByteArray>


namespace sonarphony
{

/// \brief Constructs a "master" command
class SONARPHONY_LOCAL masterCommandBuilder_t
{
public:
	~masterCommandBuilder_t ();
	masterCommandBuilder_t ();

	/// \brief Set the range
	/// \param min_ Minimum range (upper limit) [0, ?] feet
	/// \param max_ Maximum range (lower limit) [0, 240] feet
	///
	/// The maximum range for the T-Pod is 120 feet, while the T-Box is
	/// rated to 240 feet, although this has not been tested. Setting both
	/// values to 0 will enable auto-ranging.
	void setRange (unsigned min_, unsigned max_);

    /// \brief Set the desired frequency
    void setFrequency (sonarConnection_t::frequency_t freq_);

	/// \brief Build the message
	QByteArray const &build ();

private:
	QByteArray m_buffer;    ///< Message buffer

	unsigned m_minRange;    ///< Requested minimum range
	unsigned m_maxRange;    ///< Requested maximum range
	unsigned m_frequency;   ///< Requested frequency option
};

}
