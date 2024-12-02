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

#include "about.hh"
#include "playback.hh"

#include "sonarphony/sonarConnection.hh"
#include "sonarphony/logger.hh"

#include "ui_mainWindow.h"
#include "ui_preferences.h"

#include <QByteArray>
#include <QTimer>

#include <deque>


class mainWindow_t : public QMainWindow
{
	Q_OBJECT
public:
	mainWindow_t ();

private slots:
	/// \brief Handle any old message from the sonar
	/// \param msg_ Message to handle
	void handleData (QByteArray const &msg_);
	/// \brief Handle a ping message
	/// \param ping_ Ping message
	void handlePing (quint64 tstamp_, sonarphony::pingMsg_t const &ping_);

	/// \brief Handle a change in serial number
	void serialNumberChanged ();
	/// \brief Handle a change in the requested range by the user
	void rangeChanged ();

	/// \brief About menu item clicked
	/// \param checked_ Is it checked?
	void on_actionAbout_triggered (bool checked_);
	/// \brief Preferences menu item clicked
	/// \param checked_ Is it checked?
	void on_actionPreferences_triggered (bool checked_);

private:
	Ui::mainWindow m_ui;            ///< Primary form

	struct
	{
		Ui::preferences ui;     ///< Preferences form
		QDialog dialog;         ///< Enclosing dialog for form
	} m_preferences;                ///< Preferences page

	about_t m_aboutDialog;          ///< About Dialog

	/// \brief Connection to the sonar
	sonarphony::sonarConnection_t m_connection;

	/// \brief Raw data logger
	sonarphony::rawLogger_t m_rawLogger;
	/// \brief NMEA0183 data logger
	sonarphony::nmeaLogger_t m_nmeaLogger;

    playback_t m_playback;
};

