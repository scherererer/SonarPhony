# SonarPhony Daemon
BASEDIR = "../.."
include ($${BASEDIR}/project.pri)

TEMPLATE        = app
TARGET          = sonarphonyd

CONFIG  += qt
QT      -= gui
QT      += network

LINK_LOCAL(library/sonarphony, sonarphony)

HEADERS += daemon.hh

SOURCES += daemon.cc
SOURCES += main.cc
