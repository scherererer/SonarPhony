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

#include "playback.hh"

using namespace sonarphony;

#include <QtEndian>


playback_t::~playback_t()
{
}

playback_t::playback_t()
    : m_file()
    , m_timer(this)
{
}

void playback_t::setFile(QString const &file_)
{
    m_file.setFileName(file_);
    m_file.open(QIODeviceBase::ReadOnly);

    m_timer.setSingleShot(false);
    m_timer.setInterval(5); // TODO
    m_timer.setTimerType(Qt::PreciseTimer);
	connect (&m_timer, SIGNAL (timeout ()),
	         SLOT (readMore ()));

    m_timer.start();
}

void playback_t::readMore()
{
    if (! m_file.isOpen() || m_file.atEnd())
    {
        m_file.close();
        m_file.open(QIODeviceBase::ReadOnly);
        return;
    }

    QByteArray const sizeBuffer = m_file.read(4);
	unsigned const size = qFromLittleEndian<quint32>(sizeBuffer.constData()) - 8;

    QByteArray const timeBuffer = m_file.read(8);
    quint64 const time = qFromLittleEndian<quint64>(timeBuffer); // TODO

    QByteArray const dataBuffer = m_file.read(size);

    if (size<200)
        return;

    emit ping(time, pingMsg_t(dataBuffer));
}

