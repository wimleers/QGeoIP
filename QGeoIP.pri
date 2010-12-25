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


QGEOIP_DATA_DIR = $${PWD}/data

QGEOIP_DATA_FILES += \
    GeoIPCity.dat \
    GeoIPASNum.dat

# Copy all QGEOIP_DATA_FILES to the build directory when using shadow builds.
!equals($${PWD}, $${OUT_PWD}) {
    MKDIR = mkdir
    unix:COPY  = cp
    win32:COPY = copy /y

    # Create the "data" sub directory in the shadow build directory.
    QMAKE_PRE_LINK += $${MKDIR} $${OUT_PWD}/data;

    # Copy each data file.
    for(data_file, QGEOIP_DATA_FILES) {
        QMAKE_PRE_LINK += $${COPY} $${QGEOIP_DATA_DIR}/$${data_file} $${OUT_PWD}/data/$${data_file};
    }
}
