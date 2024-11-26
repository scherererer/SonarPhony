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

#include "waterfall.hh"
#include "palette.hh"

using namespace sonarphony;

#include <QPainter>

#include <algorithm>
#include <cmath>
#include <limits>
using namespace std;


///////////////////////////////////////////////////////////////////////////
namespace
{

/// \brief Number of pings to display in the waterfall
unsigned const NUM_PINGS                = 512;
/// \brief Resolution of watercolumn to display
unsigned const VERTICAL_RESOLUTION      = 800;

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

	unsigned char const * const data = reinterpret_cast<unsigned char const *>(msg_.pingData ());

	if (b >= size)
		return data[a];
	else
		return (static_cast<int> (data[a]) + data[b]) / 2.0;
}

}


///////////////////////////////////////////////////////////////////////////
waterfall_t::~waterfall_t ()
{
}

waterfall_t::waterfall_t (QWidget *parent_) :
	QLabel (parent_),
	m_pix (NUM_PINGS, VERTICAL_RESOLUTION),
	m_msgBuffer (),
	m_minRange (0),
	m_maxRange (0),
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

void waterfall_t::handlePing (sonarphony::pingMsg_t const &ping_)
{
	if (m_msgBuffer.size () + 1 > NUM_PINGS)
	{
		pingMsg_t const &remove = m_msgBuffer.front ();

		/// \todo This optimization is really an edge case, most of the
		/// time the range will be the same because I don't expect the
		/// range to change often at runtime. Probably instead could
		/// have incoming pings "vote" for the current range if they
		/// match that range, and then decrement the vote when those
		/// pings disappear. Only iterate through list if counter goes
		/// to zero, only clear votes if a wider range comes in.
		if (m_minRange == remove.minRange () ||
		    m_maxRange == remove.maxRange ())
		{
			// Skip past the outgoing ping
			deque<pingMsg_t>::iterator i = ++m_msgBuffer.begin ();

			double newMin = numeric_limits<double>::max ();
			double newMax = 0;

			for (; i != m_msgBuffer.end (); ++i)
			{
				newMin = ::min<double> (i->minRange (), newMin);
				newMax = ::max<double> (i->maxRange (), newMax);

				// Skip early if the min/max we've found to this
				// point matches our existing, since there can't
				// possibly be a larger value
				if (newMin == m_minRange &&
				    newMax == m_maxRange)
					break;
			}

			m_minRange = newMin;
			m_maxRange = newMax;
		}

		m_msgBuffer.pop_front ();
	}

	m_minRange = ::min<double> (ping_.minRange (), m_minRange);
	m_maxRange = ::max<double> (ping_.maxRange (), m_maxRange);

	m_msgBuffer.push_back (ping_);

	if (! m_timer.isActive ())
		m_timer.start ();
}

void waterfall_t::redraw ()
{
	QPainter painter (&m_pix);

	double const range = m_maxRange - m_minRange;

	for (unsigned c = 0; c < m_msgBuffer.size (); ++c)
		for (unsigned i = 0; i < VERTICAL_RESOLUTION; ++i)
		{
			double const depth =
				(static_cast<double> (i) /
				 (VERTICAL_RESOLUTION - 1))
				* range + m_minRange;

			unsigned const value = interp (m_msgBuffer[c], depth);

			if (value > 255)
				painter.setPen (
					backgrounds::list[m_backgroundIndex]
					.color);
			else
				painter.setPen (
					palettes::list[m_paletteIndex]
					.table[value]);

			painter.drawPoint (c, i);
		}

	setPixmap (m_pix);
}
