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

#include "sonarphony/sonarConnection.hh"
#include "masterCommand.hh"
#include "masterHandshake.hh"
#include "sonarHandshakeMsg.hh"
#include "sonarphony/pingMsg.hh"
#include "sonarphony/sonarMsg.hh"
using namespace sonarphony;

#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QUdpSocket>

#include <vector>
using namespace std;


///////////////////////////////////////////////////////////////////////////
namespace
{

/// \brief Sonar host name
///
/// May need to be configurable, but I doubt it
char const *HOST = "192.168.1.1";
/// \brief Sonar command port
///
/// May need to be configurable, but I doubt it
unsigned short const PORT = 5000;

}


///////////////////////////////////////////////////////////////////////////
/// \brief Private data
class sonarConnection_t::private_t
{
public:
	/// \param parent_ Parent object
	private_t (sonarConnection_t *parent_) :
		requestTimer (parent_),
		socket (parent_),
		handshakeFinished (false),
		masterFinished (false),
		serialNumber (),
		masterCommand (),
		pause(false)
	{}

	QTimer requestTimer;            ///< Timer to poll the device
	QUdpSocket socket;              ///< Socket to send/receive data on

	bool handshakeFinished;         ///< Has the handshake finished?
	bool masterFinished;            ///< Have we declared ourselves master?

	std::string serialNumber;       ///< Serial number of device

	QByteArray masterCommand;       ///< Cached command to send to device

	bool pause;

	masterCommandBuilder_t builder;
};


///////////////////////////////////////////////////////////////////////////
sonarConnection_t::~sonarConnection_t ()
{
	delete m_d;
}

sonarConnection_t::sonarConnection_t (QObject *parent_) :
	QObject (parent_),
	m_d (new private_t (this))
{
	m_d->builder.setRange (0, 9);

	m_d->masterCommand = m_d->builder.build ();

	connect (&m_d->requestTimer, SIGNAL (timeout ()),
	         SLOT (query ()));

	m_d->requestTimer.setSingleShot (false);
	m_d->requestTimer.setInterval (200);

	connect (&m_d->socket, SIGNAL (readyRead ()),
	         SLOT (handleDatagrams ()));

	m_d->socket.bind();
}

void sonarConnection_t::setRange (double min_, double max_)
{
	m_d->builder.setRange (min_, max_);
	m_d->masterCommand = m_d->builder.build ();
}

void sonarConnection_t::setFrequency (frequency_t freq_)
{
	m_d->builder.setFrequency (freq_);
	m_d->masterCommand = m_d->builder.build ();
}

string sonarConnection_t::serialNumber () const
{
	return m_d->serialNumber;
}

void sonarConnection_t::start ()
{
	m_d->requestTimer.start ();
	m_d->pause = false;
}

void sonarConnection_t::stop ()
{
	m_d->pause = true;
	m_d->handshakeFinished = false;
	m_d->masterFinished = false;
}

void sonarConnection_t::query ()
{
	QByteArray cmd;

	if (m_d->pause)
		cmd = masterHandshakeBuilder_t().buildPause();
	else if (! m_d->handshakeFinished)
		cmd = masterHandshakeBuilder_t().build();
	else if (! m_d->masterFinished)
		cmd = masterHandshakeBuilder_t().buildMaster();
	else
		cmd = m_d->masterCommand;

	m_d->socket.writeDatagram(cmd, QHostAddress(HOST), PORT);
}

void sonarConnection_t::handleDatagrams ()
{
	while (m_d->socket.hasPendingDatagrams ())
	{
		QByteArray dgram;
		QHostAddress sender;
		unsigned short port;

		dgram.resize (m_d->socket.pendingDatagramSize ());

		m_d->socket.readDatagram (dgram.data (), dgram.size (),
		                          &sender, &port);

		vector<sonarMsg_t> msgs = splitMessages (dgram);

		for (vector<sonarMsg_t>::iterator m = msgs.begin ();
		     m != msgs.end (); ++m)
		{
			switch (m->type ())
			{
			case sonarMsg_t::T_HANDSHAKE:
			    {
				sonarHandshakeMsg_t const hs (*m);

				m_d->handshakeFinished = true;
				m_d->serialNumber = hs.serialNumber ();

				emit serialNumberChanged ();
				break;
			    }
			case sonarMsg_t::T_MASTER:
			    m_d->masterFinished = true;
			    break;
			case sonarMsg_t::T_PING:
			    {
				pingMsg_t p (*m);

				emit ping (QDateTime::currentMSecsSinceEpoch(), p);
				break;
			    }
			case sonarMsg_t::T_UNKNOWN:
			case sonarMsg_t::T_INVALID:
			case sonarMsg_t::T_BUSY:
			case sonarMsg_t::T_V:
				break;
			}
		}

		emit newData (dgram);
	}
}
