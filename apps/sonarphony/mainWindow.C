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

#include "mainWindow.H"
#include "palette.H"

#include "sonarphony/pingMsg.H"
using namespace sonarphony;

#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QTime>

#include <sstream>
using namespace std;


mainWindow_t::mainWindow_t () :
	QMainWindow (),
	m_ui (),
	m_preferences (),
	m_aboutDialog (this),
	m_connection (),
	m_rawLogger (),
	m_nmeaLogger ()
{
	m_ui.setupUi (this);


	///////////////////////////////////////////////////////////////////
	m_preferences.ui.setupUi (&m_preferences.dialog);

	for (unsigned i = 0; i < backgrounds::list.size (); ++i)
		m_preferences.ui.background->addItem (
			backgrounds::list[i].name, i);
	for (unsigned i = 0; i < palettes::list.size (); ++i)
		m_preferences.ui.palette->addItem (
			palettes::list[i].name, i);


	///////////////////////////////////////////////////////////////////
	connect (m_ui.runButton, SIGNAL (clicked ()),
	         &m_connection, SLOT (start ()));
	connect (m_ui.stopButton, SIGNAL (clicked ()),
	         &m_connection, SLOT (stop ()));
	connect (m_ui.actionExit, SIGNAL (triggered ()),
	         qApp, SLOT (quit ()));

	connect (&m_connection,
	         SIGNAL (ping (sonarphony::pingMsg_t const &)),
	         m_ui.waterfall,
	         SLOT (handlePing (sonarphony::pingMsg_t const &)));
	connect (&m_connection,
	         SIGNAL (ping (sonarphony::pingMsg_t const &)),
	         m_ui.columnPulser,
	         SLOT (handlePing (sonarphony::pingMsg_t const &)));

	connect (&m_connection,
	         SIGNAL (newData (QByteArray const &)),
	         SLOT (handleData (QByteArray const &)));
	connect (&m_connection,
	         SIGNAL (ping (sonarphony::pingMsg_t const &)),
	         SLOT (handlePing (sonarphony::pingMsg_t const &)));
	connect (&m_connection, SIGNAL (serialNumberChanged ()),
	         SLOT (serialNumberChanged ()));

	connect (m_ui.minRangeSB, SIGNAL (valueChanged (int)),
	         SLOT (rangeChanged ()));
	connect (m_ui.maxRangeSB, SIGNAL (valueChanged (int)),
	         SLOT (rangeChanged ()));

	m_rawLogger.setConnection (m_connection);
	m_nmeaLogger.setConnection (m_connection);
}

void mainWindow_t::handleData (QByteArray const &msg_)
{
	sonarMsg_t const msg (msg_);

	switch (msg.type ())
	{
	case sonarMsg_t::T_UNKNOWN:
	    {
		QString const s =
			QString ("%1:  UNKOWN MESSAGE, size: %2, status: %3\n")
			.arg (QTime::currentTime ().toString ("HH:mm:ss.zzz"))
			.arg (msg.size ())
			.arg (msg.status ());

		m_ui.output->appendPlainText (s);
		break;
	    }
	case sonarMsg_t::T_HANDSHAKE:
		m_ui.output->appendPlainText ("Handshake");
		break;
	case sonarMsg_t::T_PING:
		break;
	case sonarMsg_t::T_BUSY:
		break;
	}
}

void mainWindow_t::handlePing (pingMsg_t const &ping_)
{
	m_ui.depth->setText (QString::number (ping_.depth (), 'f', 1));

	m_ui.temperature->setText (
		QString::number (ping_.temperature ()) + " C");

	m_ui.batteryCharge->setValue (ping_.batteryLevel ());
	m_ui.batteryVoltage->setText (
		QString::number (ping_.batteryVoltage (), 'f', 1) + " V");

	m_ui.minRange->setText (QString::number (ping_.minRange ()) + " ft");
	m_ui.maxRange->setText (QString::number (ping_.maxRange ()) + " ft");
}

void mainWindow_t::rangeChanged ()
{
	if (m_ui.minRangeSB->value () >= m_ui.maxRangeSB->value ())
		m_ui.minRangeSB->setValue (m_ui.maxRangeSB->value () - 1);

	m_connection.setRange (m_ui.minRangeSB->value (),
	                       m_ui.maxRangeSB->value ());
}

void mainWindow_t::serialNumberChanged ()
{
	setWindowTitle (("SonarPhony (" + m_connection.serialNumber () +
	                 ")").c_str ());
}

void mainWindow_t::on_actionAbout_triggered (bool checked_)
{
	Q_UNUSED (checked_);

	m_aboutDialog.open ();
}

void mainWindow_t::on_actionPreferences_triggered (bool checked_)
{
	Q_UNUSED (checked_);

	m_preferences.ui.logDirectoryEdit->setText (
		m_rawLogger.directory ().c_str ());

	if (! m_preferences.dialog.exec ())
		return;

	unsigned const backgroundIndex = m_preferences.ui.background->itemData (
		m_preferences.ui.background->currentIndex ()).toUInt ();
	unsigned const paletteIndex = m_preferences.ui.palette->itemData (
		m_preferences.ui.palette->currentIndex ()).toUInt ();

	m_ui.waterfall->setBackgroundIndex (backgroundIndex);
	m_ui.waterfall->setPaletteIndex (paletteIndex);

	m_ui.columnPulser->setBackgroundIndex (backgroundIndex);
	m_ui.columnPulser->setPaletteIndex (paletteIndex);

	m_rawLogger.setEnabled (m_preferences.ui.enableRawLogs->isChecked ());
	m_nmeaLogger.setEnabled (m_preferences.ui.enableNmeaLogs->isChecked ());
}

