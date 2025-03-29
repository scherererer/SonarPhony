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

#include "sonarphony/logger.hh"
#include "sonarphony/pingMsg.hh"
#include "sonarphony/sonarConnection.hh"
using namespace sonarphony;

#include <QDir>
#include <QDateTime>
#include <QtEndian>


///////////////////////////////////////////////////////////////////////////
namespace
{

// Limit file size to a little bit under 2 GiB
unsigned const MAX_FILE_SIZE    = 1024 * 1024 * 2000;

/// \brief Calculate a NMEA0183 checksum
/// \param buffer_ Buffer to calculate sum for
/// \todo This should probably be moved someplace common, as the daemon uses
/// this as well. That being said I feel like this is somewhat out of the scope
/// of this project so I don't want this to expand too much into a big nmea0183
/// implementation.
///
/// Operates until it runs into a null terminating character.
inline int nmea0183checksum (QByteArray const &buffer_)
{
	int cksum = 0;
	unsigned offset = 0;

	while (buffer_[offset])
		cksum ^= buffer_[offset++];

	return cksum;
}

}


///////////////////////////////////////////////////////////////////////////
logger_t::~logger_t ()
{
	m_currentFile.close ();
}

void logger_t::setEnabled (bool enable_)
{
	m_enabled = enable_;

	if (! m_enabled)
		m_currentFile.close ();
}

void logger_t::setDirectory (std::string const &directory_)
{
	m_directory = directory_;

	QDir const dir;

	if (! dir.exists (m_directory.c_str ()))
		dir.mkpath (m_directory.c_str ());
}

void logger_t::log (QByteArray const &data_)
{
	if (! m_enabled)
		return;

	if (! m_currentFile.isOpen () ||
	    m_currentFile.size () + data_.size () > MAX_FILE_SIZE)
	{
		m_currentFile.close ();

		QString const fileName = m_directory.c_str () +
			QDateTime::currentDateTime ()
			.toString ("/yyyy-MM-ddTHH_mm_ss.zzz.") +
			m_extension.c_str ();

		m_currentFile.setFileName (fileName);

		if (! m_currentFile.open (QIODevice::WriteOnly))
		{
			qCritical ("Failed to open file for logging '%s'",
			           qPrintable (fileName));
			return;
		}

		qDebug ("Opened new log file '%s'", qPrintable (fileName));
	}

	m_currentFile.write (data_);
	/// \todo Should probably throttle this a little bit
	m_currentFile.flush ();
}

logger_t::logger_t (std::string const &extension_) :
	m_extension (extension_),
	m_directory (),
	m_enabled (false),
	m_currentFile ()
{
	setDirectory ((QDir::home ().absoluteFilePath ("sonarphony"))
	              .toStdString ());
}

rawLogger_t::rawLogger_t () :
	logger_t ("sonarphony.raw"),
	m_buffer ()
{
}

void rawLogger_t::setConnection (sonarConnection_t &connection_)
{
	connect (&connection_, SIGNAL (newData (QByteArray const &)),
	         SLOT (handleData (QByteArray const &)));
}

void rawLogger_t::handleData (QByteArray const &data_)
{
	m_buffer.resize (12);

	qToLittleEndian<quint32> (
		(data_.size()+8),
		reinterpret_cast<uchar *> (m_buffer.data()));

	qToLittleEndian<quint64> (
		QDateTime::currentMSecsSinceEpoch(),
		reinterpret_cast<uchar *> (m_buffer.data()+4));

	m_buffer.append (data_);

	log (m_buffer);
}

nmeaLogger_t::nmeaLogger_t () :
	logger_t ("sonarphony.nmea0183"),
	m_buffer ()
{
	m_buffer.reserve (128);
}

void nmeaLogger_t::setConnection (sonarConnection_t &connection_)
{
	connect (&connection_, &sonarConnection_t::ping,
	         this, &nmeaLogger_t::handlePing);
}

void nmeaLogger_t::handlePing (quint64 tstamp_, pingMsg_t const &ping_)
{
    (void) tstamp_;

	// NMEA0183 'specification' taken from:
	// http://www.nmea.de/nmea0183datensaetze.html
	// https://en.wikipedia.org/wiki/NMEA_0183

	m_buffer.resize (128);

	/// \note I've fixed the mounting offset here to 0, in the future
	/// it might be nice to make this configurable
	int chars = snprintf (m_buffer.data (), m_buffer.size (),
	                      "$SDDPT,%.1f,0.0*", ping_.depth ());

	if (chars <= 0)
		return;

	int total = chars;

	// Add the checksum and the end of line characters
	chars = snprintf (m_buffer.data () + chars, m_buffer.size () - chars,
	                  "%02X\r\n", nmea0183checksum (m_buffer));

	if (chars <= 0)
		return;

	total += chars;

	m_buffer.resize (total);

	log (m_buffer);
}

