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

#include "mainWindow.hh"

#include <QApplication>
#include <QStyleFactory>

#include <iostream>
using namespace std;

int main (int argc_, char **argv_)
{
	QApplication app (argc_, argv_);

	app.setApplicationName (argv_[0]);
	app.setApplicationVersion (SONARPHONY_VERSION);

	// Nicked this style snippet from:
	// https://gist.github.com/QuantumCD/6245215
	app.setStyle (QStyleFactory::create ("Fusion"));

	QPalette darkPalette;
	darkPalette.setColor (QPalette::Window, QColor (53,53,53));
	darkPalette.setColor (QPalette::WindowText, Qt::white);
	darkPalette.setColor (QPalette::Base, QColor (25,25,25));
	darkPalette.setColor (QPalette::AlternateBase, QColor (53,53,53));
	darkPalette.setColor (QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor (QPalette::ToolTipText, Qt::white);
	darkPalette.setColor (QPalette::Text, Qt::white);
	darkPalette.setColor (QPalette::Button, QColor (53,53,53));
	darkPalette.setColor (QPalette::ButtonText, Qt::white);
	darkPalette.setColor (QPalette::BrightText, Qt::red);
	darkPalette.setColor (QPalette::Link, QColor (42, 130, 218));

	darkPalette.setColor (QPalette::Highlight, QColor (42, 130, 218));
	darkPalette.setColor (QPalette::HighlightedText, Qt::black);

	app.setPalette (darkPalette);

	app.setStyleSheet ("QToolTip { color: #ffffff; "
	                   "background-color: #2a82da; "
	                   "border: 1px solid white; }");
	// End of snippet

	mainWindow_t main;

	main.show ();

	return app.exec ();
}
