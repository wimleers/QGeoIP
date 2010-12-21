#include "QGeoIP.h"
#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <QTime>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QTextStream cout(stdout);
    QTime timer;
    int timePassed;

    QGeoIP geoIP;
    geoIP.openDatabases("./GeoIPCity.dat", "./GeoIPISP.dat");
    QGeoIPRecord record1, record2;

    QHostAddress ip1, ip2;
    ip1.setAddress("24.24.24.24");
    ip2.setAddress("84.193.172.100");

    timer.start();
    record1 = geoIP.recordByAddr(ip1);
    record2 = geoIP.recordByAddr(ip2);
    timePassed = timer.elapsed();

    qDebug() << record1 << endl
             << record2;

    cout << QString("Duration: %1 ms.")
            .arg(timePassed)
            << endl;
}
