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
HEADERS += $${_HDIR}/logger.H
HEADERS += $${_HDIR}/pingMsg.H
HEADERS += $${_HDIR}/sonarConnection.H
HEADERS += $${_HDIR}/sonarHandshakeMsg.H
HEADERS += $${_HDIR}/sonarMsg.H

# Private Headers
HEADERS += masterCommand.H
HEADERS += masterHandshake.H
HEADERS += msgPrivate.H

# Sources
SOURCES += logger.C
SOURCES += masterCommand.C
SOURCES += masterHandshake.C
SOURCES += pingMsg.C
SOURCES += sonarConnection.C
SOURCES += sonarHandshakeMsg.C
SOURCES += sonarMsg.C

#INSTALLS += $${TARGET}
