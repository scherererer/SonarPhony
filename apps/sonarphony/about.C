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

#include "about.H"


///////////////////////////////////////////////////////////////////////////
about_t::~about_t ()
{
}

about_t::about_t (QWidget *parent_) :
	QDialog (parent_),
	m_ui ()
{
	m_ui.setupUi (this);

	m_ui.versionLabel->setText (SONARPHONY_VERSION);
	m_ui.copyrightLabel->setText (SONARPHONY_COPYRIGHT_YEARS);
}
