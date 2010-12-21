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

OTHER_FILES += GeoIPCity.dat

# Copy all OTHER_FILES to the build directory when using shadow builds.
!equals($${PWD}, $${OUT_PWD}) {
    unix:COPY  = cp
    win32:COPY = copy /y
    for(other_file, OTHER_FILES) {
          QMAKE_PRE_LINK += $${COPY} $${PWD}/$${other_file} $${OUT_PWD}/$${other_file};
    }
}
