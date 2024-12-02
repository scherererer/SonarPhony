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

#include "sonarphony/pingMsg.hh"

#include <QFile>
#include <QTimer>


class playback_t : public QObject
{
	Q_OBJECT
public:
    virtual ~playback_t();
    playback_t();

    void setFile(QString const &file_);

signals:
	/// \brief A new ping message has been returned
	/// \param tstamp_ Time in seconds since the epoch
	/// \param ping_ The ping message
	void ping(quint64 tstamp_, sonarphony::pingMsg_t const &ping_);

private slots:
    void readMore();

private:
    QFile m_file;
    QTimer m_timer;
};
