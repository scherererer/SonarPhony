# SonarPhony GUI
BASEDIR = "../.."
include ($${BASEDIR}/project.pri)

TEMPLATE        = app
TARGET          = sonarphony

CONFIG  += qt
QT      += gui
QT      += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LINK_LOCAL(library/sonarphony, sonarphony)

HEADERS += about.hh
HEADERS += columnPulser.hh
HEADERS += mainWindow.hh
HEADERS += palette.hh
HEADERS += waterfall.hh

SOURCES += about.cc
SOURCES += columnPulser.cc
SOURCES += main.cc
SOURCES += mainWindow.cc
SOURCES += palette.cc
SOURCES += waterfall.cc

FORMS   += about.ui
FORMS   += mainWindow.ui
FORMS   += preferences.ui
