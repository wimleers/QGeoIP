DEPENDPATH += ..
INCLUDEPATH += ..
include("../QGeoIP.pri")

SOURCES += sample.cpp

CONFIG -= debug
CONFIG += release
macx {
  CONFIG -= app_bundle
}

OTHER_FILES += ../Tests/GeoIPCity.dat

# Copy all OTHER_FILES to the build directory when using shadow builds.
!equals($${PWD}, $${OUT_PWD}) {
    unix:COPY  = cp
    win32:COPY = copy /y
    for(other_file, OTHER_FILES) {
          QMAKE_PRE_LINK += $${COPY} $${PWD}/$${other_file} $${OUT_PWD}/$${other_file};
    }
}
