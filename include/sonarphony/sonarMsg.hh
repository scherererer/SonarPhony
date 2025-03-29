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

#include <string>
#include <vector>


namespace sonarphony
{

/// \brief Base class for any messages coming from the sonar
class sonarMsg_t
{
public:
	~sonarMsg_t ();
	sonarMsg_t ();
	/// \param buffer_ Buffer containing a single complete message
	explicit sonarMsg_t (QByteArray const &buffer_);
	/// \brief Copy constructor
	/// \param other_ Message to copy
	sonarMsg_t (sonarMsg_t const &other_);
	sonarMsg_t& operator= (const sonarMsg_t&) = default;

	/// \brief Is this a valid message?
	bool isValid () const;

	/// \brief Status information
	enum status_t
	{
		/// \brief This status code hasn't been seen before!
		S_UNKNOWN       = 0,

		/// \brief The unit is not ready/able to send a ping
		S_BUSY          = 1,
		/// \brief The unit is sending valid messages
		S_READY         = 2,
	};

	/// \brief Get the status code from the sonar
	status_t status () const;

	/// \brief Size of the UDP packet
	unsigned size () const
		{ return m_buffer.size (); }
	/// \brief Size as given in the body of the packet
	///
	/// Interestingly, some captured packets appear to show two messages
	/// in the same packet, and yet this value is constant for both of the
	/// contained messages. This is either a bug or an indicator of message
	/// type, and it is merely a peculiarity that they chose the typical
	/// message size as an indicator of type.
	///
	/// \sa sonarMsg_t::type ()
	unsigned reportedSize () const;

	/// \brief Get the message's function code
	char functionCode () const;

	/// \brief Enumeration of all possible message types
	enum type_t
	{
		/// \brief Unknown message type!
		///
		/// If you see one of these you should report it so we can
		/// figure out what it means!
		T_UNKNOWN       = 0,
		/// \brief Cannot determine type because message is invalid
		T_INVALID,
		/// \brief Handshake message type
		/// \sa sonarHandshakeMsg_t
		T_HANDSHAKE,
		/// \brief Ping message type
		/// \sa pingMsg_t
		T_PING,
		/// \brief Busy message type
		/// \sa sonarMsg_t
		T_BUSY,
		/// \brief "V" message type (unknown), I think it is a pause
		T_V,
		/// \brief Master message, for taking over as master
		T_MASTER,
	};

	/// \brief Get the message type
	///
	/// At the moment, this is determining the message type based on the
	/// \c reportedSize field. This is because even though some captures
	/// have shown packets containing 2 ping messages yet still the same
	/// total size of 340 bytes, each message's header appears to contain
	/// the value 340 in the \c reportedSize. This value is therefore
	/// considered a stable indicator of message type.
	///
	/// 10  Busy
	/// 32  Handshake
	/// 340 Ping
	///
	/// If you ever encounter a message of a different size, please report
	/// it so we can figure out what it is! Bonus points for including
	/// contextual information e.g. from the manufacturer's app.
	///
	/// \sa sonarMsg_t::reportedSize ()
	type_t type () const;

protected:
	QByteArray m_buffer;    ///< Message buffer
};

/// \brief Split out all messages in the given buffer
/// \param buffer_ Incoming buffer
extern std::vector<sonarMsg_t> splitMessages (QByteArray const &buffer_);

/// \brief Print the message type out as a string
/// \param type_ Type of message
extern std::string toString (sonarMsg_t::type_t type_);

}
