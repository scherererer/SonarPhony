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

#include "sonarphony/global.hh"

#include <QByteArray>
#include <QFile>

#include <string>

namespace sonarphony
{

class sonarConnection_t;
class pingMsg_t;

/// \brief Base class for logging objects
class SONARPHONY_EXPORT logger_t : public QObject
{
	Q_OBJECT
public:
	virtual ~logger_t ();

	/// \brief Enable or disable the logger
	/// \param enable_ Set to true to enable logging
	///
	/// The logger is disabled by default.
	void setEnabled (bool enable_);

	/// \brief Set the logging directory
	/// \param directory_ Directory to use
	///
	/// By default this is set to '~/sonarphony' on unix systems
	void setDirectory (std::string const &directory_);

	/// \brief Get the logging directory
	std::string const &directory () const
		{ return m_directory; }

protected:
	/// \param extension_ File name extension
	logger_t (std::string const &extension_);

	/// \brief Log a blob of data to file
	/// \param data_ Data blob to be logged. Must be << 2GiB
	void log (QByteArray const &data_);

private:
	std::string const m_extension;  ///< File name extension
	std::string m_directory;        ///< Base logging directory

	bool m_enabled;                 ///< Is this logger enabled?

	QFile m_currentFile;            ///< Currently open log file
};

/// \brief Log data raw
///
/// All data is encoded with a 4 byte unsigned integer to indicate the size of
/// the message blob (LE), followed by the blob itself.
class SONARPHONY_EXPORT rawLogger_t : public logger_t
{
	Q_OBJECT
public:
	~rawLogger_t ();
	rawLogger_t ();

	/// \brief Set the connection to log. Should only be called once.
	/// \param connection_ Connection to log
	void setConnection (sonarConnection_t &connection_);

private slots:
	/// \brief Handle a raw blob of data
	/// \param data_ Data to be handled
	void handleData (QByteArray const &data_);

private:
	QByteArray m_buffer;    ///< Buffer to encode to
};

/// \brief Log data as NMEA0183
///
/// Logs DPT messages to file
class SONARPHONY_EXPORT nmeaLogger_t : public logger_t
{
	Q_OBJECT
public:
	~nmeaLogger_t ();
	nmeaLogger_t ();

	/// \brief Set the connection to log. Should only be called once.
	/// \param connection_ Connection to log
	void setConnection (sonarConnection_t &connection_);

private slots:
	/// \brief Handle a ping message
	/// \param ping_ Ping message
	void handlePing (quint64 tstamp_, sonarphony::pingMsg_t const &ping_);

private:
	QByteArray m_buffer;    ///< Buffer to encode to
};

}
