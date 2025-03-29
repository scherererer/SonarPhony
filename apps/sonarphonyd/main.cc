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

#include "daemon.hh"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

#include <unistd.h>

#include <string>
#include <iostream>
using namespace std;

int main (int argc_, char **argv_)
{
	QCoreApplication app(argc_, argv_);

	app.setApplicationName(argv_[0]);
	app.setApplicationVersion(SONARPHONY_VERSION);

	QCommandLineParser parser;
	parser.setApplicationDescription("SonarPhony Daemon");
	QCommandLineOption const helpOption = parser.addHelpOption();
	QCommandLineOption const versOption = parser.addVersionOption();

	QCommandLineOption udpPortOption(QStringList() << "u" << "udp-port",
		QString("Configure to broadcast UDP to <port>"),
		QString("port"), QString("0"));
	parser.addOption(udpPortOption);

	QCommandLineOption rawLogOption(QStringList() << "r" << "raw-log",
		QString("Log raw data to <directory>"),
		QString("directory"));
	parser.addOption(rawLogOption);

	if (!parser.parse(QCoreApplication::arguments()))
	{
		cerr << parser.errorText().toStdString();
		return 1;
	}

	int const udpPort = stoi(parser.value(udpPortOption).toStdString());

	if (parser.isSet(helpOption))
	{
		parser.showHelp();
		//     << "This program comes with ABSOLUTELY NO WARRANTY.\n"
		//     << "This is free software, and you are welcome to "
		//        "redistribute it\n"
		//     << "under the terms of the GPL v3.0. If a copy wasn't "
		//     << "provided to you \n"
		//     << "then please see <http://www.gnu.org/licenses/>\n"
		return 0;
	}

	if(parser.isSet(versOption))
	{
		parser.showVersion();
		return 0;
	}

	daemon_t daemon;

	daemon.initialize(udpPort);

	if (parser.isSet(rawLogOption))
		daemon.rawLog(parser.value(rawLogOption).toStdString());

	return app.exec();
}

