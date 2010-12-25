DEPENDPATH += ..
INCLUDEPATH += ..
include("../QGeoIP.pri")

SOURCES += sample.cpp

CONFIG -= debug
CONFIG += release
macx {
  CONFIG -= app_bundle
}
