TEMPLATE = lib
VERSION = 1.4.7b9
DESTDIR = $${PWD}/build
CONFIG += static create_prl

# This is a library that doesn't use Qt at all.
QT -= core gui
CONFIG -= qt exceptions thread moc uic


DEFINES += GEOIPDATADIR=\\\"./data\\\"

HEADERS += \
    GeoIP.h \
    GeoIP_internal.h \
    GeoIPCity.h \
    GeoIPUpdate.h \
    global.h \
    md5.h \
    types .h
SOURCES += \
    GeoIP.c \
    GeoIPCity.c \
    GeoIPUpdate.c \
    md5.c \
    regionName.c \
    timeZone.c

CONFIG(debug, debug|release) {
    TARGET = GeoIPd
} else {
    TARGET = GeoIP
}
