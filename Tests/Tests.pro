DEPENDPATH += ..
INCLUDEPATH += ..
include("../QGeoIP.pri")

CONFIG += qtestlib
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc

macx {
  CONFIG -= app_bundle
}
TARGET = TestQGeoIP

HEADERS += TestQGeoIP.h
SOURCES += TestQGeoIP.cpp

