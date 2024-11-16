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

#include <QString>
#include <QColor>

#include <vector>

namespace palettes
{

struct palette_t
{
	QString name;
	QColor table[256];
};

extern std::vector<palette_t> list;

}

namespace backgrounds
{

struct background_t
{
	background_t (QString name_, QColor color_) :
		name (name_), color (color_)
	{}

	QString name;
	QColor color;
};

extern std::vector<background_t> list;

};

