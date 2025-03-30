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
#include "sonarphony/sonarMsg.hh"

#include <QByteArray>
#include <QMetaType>


namespace sonarphony
{

/// \brief Message encapsulating a ping
///
/// Messages must have a ready status
class SONARPHONY_EXPORT pingMsg_t : public sonarMsg_t
{
public:
	~pingMsg_t ();
	pingMsg_t ();

	/// \param buffer_ Buffer containing a single complete message
	explicit pingMsg_t (QByteArray const &buffer_);
	/// \brief Copy constructor
	/// \param msg_ Message to be copied
	explicit pingMsg_t (sonarMsg_t const &msg_);
	/// \brief Copy constructor
	/// \param msg_ Message to be copied
	pingMsg_t (pingMsg_t const &msg_);
	pingMsg_t& operator= (const pingMsg_t&) = default;

	/// \brief Get the minimum range (upper limit) in feet
	unsigned minRange () const;
	/// \brief Get the maximum range (upper limit) in feet
	unsigned maxRange () const;

	/// \brief Get the measured depth in feet
	double depth () const;

	/// \brief Get the temperature in degrees celcius
	double temperature () const;

	/// \brief Get the raw battery voltage
	/// \note This may give unexpected results on the SP200/SP300 models
	/// since they don't have a battery
	///
	/// Product sheet says the T-POD has a 3.7V Li-Ion battery. Observed
	/// a peak of 4.2 V. Assumes a 2.4 V minimum.
	double batteryVoltage () const;
	/// \brief Get the approximate battery level
	/// \returns Approximate battery level [0, 100]
	///
	/// This value inevitably will need some calibration. For now just does
	/// a simple linear interpolation between a hardcoded max and min
	/// battery voltage.
	unsigned batteryLevel () const;

	/// \brief Points to the ping data buffer
	char const *pingData () const;
	/// \brief Size of ping data buffer
	unsigned pingSize () const;
};

}

Q_DECLARE_METATYPE (sonarphony::pingMsg_t);
