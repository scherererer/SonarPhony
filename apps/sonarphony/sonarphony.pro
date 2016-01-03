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

HEADERS += about.H
HEADERS += columnPulser.H
HEADERS += mainWindow.H
HEADERS += palette.H
HEADERS += waterfall.H

SOURCES += about.C
SOURCES += columnPulser.C
SOURCES += main.C
SOURCES += mainWindow.C
SOURCES += palette.C
SOURCES += waterfall.C

FORMS   += about.ui
FORMS   += mainWindow.ui
FORMS   += preferences.ui
