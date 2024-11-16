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

#include <QLabel>
#include <QPixmap>
#include <QTimer>

#include <deque>


/// \brief Waterfall widget
class waterfall_t : public QLabel
{
	Q_OBJECT
public:
	~waterfall_t ();
	/// \param parent_ Parent widget
	explicit waterfall_t (QWidget *parent_ = 0);

	/// \brief Set the index of the background color to use
	/// \param index_ Color index
	void setBackgroundIndex (unsigned index_)
		{ m_backgroundIndex = index_; }
	/// \brief Set the index of the color palette to use
	/// \param index_ Palette index
	void setPaletteIndex (unsigned index_)
		{ m_paletteIndex = index_; }

public slots:
	/// \brief Handle a ping message
	/// \param ping_ Ping message to be handled
	void handlePing (sonarphony::pingMsg_t const &ping_);

private slots:
	/// \brief Redraw the waterfall
	void redraw ();

private:
	/// \brief Pixmap to render to
	QPixmap m_pix;
	/// \brief Buffer of pings to be drawn
	std::deque<sonarphony::pingMsg_t> m_msgBuffer;
	/// \brief Smallest end of range across all pings in buffer
	double m_minRange;
	/// \brief Largest end of range across all pings in buffer
	double m_maxRange;

	unsigned m_backgroundIndex;     ///< Index of background color to use
	unsigned m_paletteIndex;        ///< Index of color palette to use

	QTimer m_timer;                 ///< Timer to throttle redraws
};
