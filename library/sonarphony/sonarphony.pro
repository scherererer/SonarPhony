# SonarPhony
BASEDIR = "../.."
include ($${BASEDIR}/project.pri)

DEFINES += SONARPHONY_LIBRARY

TEMPLATE        = lib
TARGET          = sonarphony

CONFIG  += qt
QT      -= gui
QT      += network

# Exposed headers
HEADERS += $${_HDIR}/logger.hh
HEADERS += $${_HDIR}/pingMsg.hh
HEADERS += $${_HDIR}/sonarConnection.hh
HEADERS += $${_HDIR}/sonarMsg.hh

# Private Headers
HEADERS += masterCommand.hh
HEADERS += masterHandshake.hh
HEADERS += msgPrivate.hh
HEADERS += sonarHandshakeMsg.hh

# Sources
SOURCES += logger.cc
SOURCES += masterCommand.cc
SOURCES += masterHandshake.cc
SOURCES += pingMsg.cc
SOURCES += sonarConnection.cc
SOURCES += sonarHandshakeMsg.cc
SOURCES += sonarMsg.cc

#INSTALLS += $${TARGET}
