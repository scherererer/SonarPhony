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

#include "columnPulser.H"
#include "palette.H"

using namespace sonarphony;

#include <QPainter>

#include <algorithm>
#include <cmath>
#include <limits>
using namespace std;


///////////////////////////////////////////////////////////////////////////
namespace
{

/// \brief Horizontile resolution
unsigned const HORIZONTILE_RESOLUTION   = 48;
/// \brief Resolution of watercolumn to display
unsigned const VERTICAL_RESOLUTION      = 500;

/// \brief Interpolate a ping intensity at the given depth
/// \param msg_ Ping message to get column from
/// \param depth_ Depth to get intensity at
/// \returns An interpolated intensity value in [0,255] or a larger value if
/// no value could be calculated
unsigned interp (pingMsg_t const &msg_, double const depth_)
{
	double const minRange = msg_.minRange ();
	double const maxRange = msg_.maxRange ();

	// Invalid ping
	if (maxRange <= minRange)
		return -1;
	// Queried depth not contained in this ping
	if (depth_ < minRange || depth_ > maxRange)
		return -1;

	unsigned const size = msg_.pingSize ();
	double const pos =
		((depth_ - minRange) / (maxRange - minRange)) * (size - 1);
	unsigned const a = floor (pos);
	unsigned const b = ceil (pos);

	Q_ASSERT (a <= b);
	Q_ASSERT (a < size);

	char const * const data = msg_.pingData ();

	if (b >= size)
		return data[a];
	else
		return (static_cast<int> (data[a]) + data[b]) / 2.0;
}

}


///////////////////////////////////////////////////////////////////////////
columnPulser_t::~columnPulser_t ()
{
}

columnPulser_t::columnPulser_t (QWidget *parent_) :
	QLabel (parent_),
	m_pix (HORIZONTILE_RESOLUTION, VERTICAL_RESOLUTION),
	m_lastMsg (),
	m_backgroundIndex (0),
	m_paletteIndex (0),
	m_timer (this)
{
	m_pix.fill (qRgb (255,255,255));

	connect (&m_timer, SIGNAL (timeout ()),
	         SLOT (redraw ()));

	m_timer.setSingleShot (true);
	m_timer.setInterval (100);
}

void columnPulser_t::handlePing (sonarphony::pingMsg_t const &ping_)
{
	m_lastMsg = ping_;

	if (! m_timer.isActive ())
		m_timer.start ();
}

void columnPulser_t::redraw ()
{
	QPainter painter (&m_pix);

	double const range = m_lastMsg.maxRange () - m_lastMsg.minRange ();

	for (unsigned i = 0; i < VERTICAL_RESOLUTION; ++i)
	{
		double const depth =
			(static_cast<double> (i) / (VERTICAL_RESOLUTION - 1))
			* range + m_lastMsg.minRange ();

		unsigned const value = interp (m_lastMsg, depth);

		if (value > 255)
			painter.setPen (
				backgrounds::list[m_backgroundIndex]
				.color);
		else
			painter.setPen (
				palettes::list[m_paletteIndex]
				.table[value]);

		painter.drawLine (0, i, HORIZONTILE_RESOLUTION-1, i);
	}

	setPixmap (m_pix);
}
