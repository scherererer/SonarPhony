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

#include "palette.hh"
using namespace palettes;
using namespace backgrounds;

using namespace std;


namespace
{

///////////////////////////////////////////////////////////////////////////
void createStriped (palette_t &p_)
{
	p_.name = "Striped";

	for (unsigned i = 0; i < 64; ++i)
		p_.table[i] = QColor (0, i * 4, 255 - i * 4);
	for (unsigned i = 0; i < 64; ++i)
		p_.table[i + 64] = QColor (i * 4, 255 - i * 4, 0);
	for (unsigned i = 0; i < 64; ++i)
		p_.table[i + 128] = QColor (255 - i * 4, 0, i * 4);
	for (unsigned i = 0; i < 64; ++i)
		p_.table[i + 192] = QColor (i * 4, i * 4, 255 - i * 4);
}

void createRed (palette_t &p_)
{
	p_.name = "Red";

	for (unsigned i = 0; i < 256; ++i)
		p_.table[i] = QColor (i, 0, 0);
}

void createDusk (palette_t &p_)
{
	p_.name = "Dusk";

	for (unsigned i = 0; i < 256; ++i)
		p_.table[i] = QColor (i, 0, 255 - i);
}

vector<palette_t> createPaletteList ()
{
	vector<palette_t> list;

	list.resize (3);

	createStriped (list[0]);
	createRed (list[1]);
	createDusk (list[2]);

	return list;
}


///////////////////////////////////////////////////////////////////////////
vector<background_t> createBackgroundList ()
{
	vector<background_t> list;

	list.push_back (background_t ("Black", QColor (0, 0, 0),
	                              QColor (255, 255, 255)));
	list.push_back (background_t ("White", QColor (255, 255, 255),
	                              QColor (0, 0, 0)));

	return list;
}


}

vector<palette_t> palettes::list = createPaletteList ();
vector<background_t> backgrounds::list = createBackgroundList ();

