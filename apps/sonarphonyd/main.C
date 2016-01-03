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

#include "daemon.H"

#include <QCoreApplication>

#include <unistd.h>

#include <string>
#include <iostream>
using namespace std;

int main (int argc_, char **argv_)
{
	QCoreApplication app (argc_, argv_);

	app.setApplicationName (argv_[0]);
	app.setApplicationVersion (SONARPHONY_VERSION);

	string configFile;
	bool usageError = false;
	int opt = '?';

	while ((opt = getopt (argc_, argv_, "c:?h")) != -1)
		switch (opt)
		{
		case 'c':
			configFile = optarg;
			break;
		case '?':
		case 'h':
			usageError = true;
			break;
		}

	if (configFile.empty ())
		usageError = true;

	if (usageError)
	{
		cerr << "SonarPhony Daemon v. " << SONARPHONY_VERSION
		     << "  Copyright (C) " << SONARPHONY_COPYRIGHT_YEARS
		     << " " << SONARPHONY_AUTHOR << "\n"
		     << "This program comes with ABSOLUTELY NO WARRANTY.\n"
		     << "This is free software, and you are welcome to "
		        "redistribute it\n"
		     << "under the terms of the GPL v3.0. If a copy wasn't "
		     << "provided to you \n"
		     << "then please see <http://www.gnu.org/licenses/>\n"
		     << "\nUsage: " << argv_[0] << " -c [config file]\n"
		     << "\n"
		     << "Options:\n"
		     << "-c [config file]\n"
		     << "\tRun using the config file given\n"
		     << "\n-?, -h\n"
		     << "\tShow this information\n";
		return 1;
	}

	daemon_t daemon;

	daemon.initialize ();

	return app.exec ();
}

