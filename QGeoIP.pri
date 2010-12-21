QT += core network
QT -= gui

# Disable qDebug() output when in release mode.
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

# Add a DEBUG define when in debug mode.
CONFIG(debug, debug|release):DEFINES += DEBUG

# Link against MaxMind's GeoIP library.
INCLUDEPATH += $${PWD}/libGeoIP
QMAKE_LIBDIR += $${PWD}/libGeoIP/build
CONFIG += link_prl
CONFIG(debug, debug|release):LIBS  += -lGeoIPd
else:LIBS  += -lGeoIP

HEADERS += QGeoIP.h
SOURCES += QGeoIP.cpp


# Copy all OTHER_FILES to the build directory when using shadow builds.
!equals($${PWD}, $${OUT_PWD}) {
    unix:COPY  = cp
    win32:COPY = copy /y
    for(other_file, OTHER_FILES) {
          QMAKE_PRE_LINK += $${COPY} $${PWD}/$${other_file} $${OUT_PWD}/$${other_file};
    }
}
