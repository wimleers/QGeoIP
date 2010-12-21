#ifndef TESTQGEOIP_H
#define TESTQGEOIP_H

#include <QtTest/QtTest>
#include <QHostAddress>
#include "QGeoIP.h"

class TestQGeoIP: public QObject {
    Q_OBJECT

private slots:
    void countryCodeByAddr();
    void countryCodeByAddr_data();
    void recordByAddr();
    void recordByAddr_data();

private:
    QGeoIP * geoIP;
};

#endif // TESTQGEOIP_H
