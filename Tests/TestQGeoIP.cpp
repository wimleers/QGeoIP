#include "TestQGeoIP.h"

QTEST_APPLESS_MAIN(TestQGeoIP);

void TestQGeoIP::recordByAddr() {
    static QHostAddress ip;
    static QGeoIPRecord record;

    this->geoIP = new QGeoIP();
    QVERIFY(this->geoIP->openDatabases("./GeoIPCity.dat", "./GeoIPISP.dat") == true);

    QFETCH(QString, ipString);
    ip.setAddress(ipString);

    record = this->geoIP->recordByAddr(ip);

    QTEST(record.country, "country");
    QTEST(record.region, "region");
    QTEST(record.city, "city");
    QTEST(record.isp, "isp");
    QTEST(record.timeZone, "timeZone");
    QTEST(record.lat, "lat");
    QTEST(record.lon, "lon");
    QTEST(QString(record.countryCode), "countryCode");
    QTEST(QString(record.continentCode), "continentCode");

    delete this->geoIP;
}

void TestQGeoIP::recordByAddr_data() {
    QTest::addColumn<QString>("ipString");
    QTest::addColumn<QString>("country");
    QTest::addColumn<QString>("region");
    QTest::addColumn<QString>("city");
    QTest::addColumn<QString>("isp");
    QTest::addColumn<QString>("timeZone");
    QTest::addColumn<float>("lat");
    QTest::addColumn<float>("lon");
    QTest::addColumn<QString>("countryCode");
    QTest::addColumn<QString>("continentCode");

    QTest::newRow("New York, U.S.A")
            << "24.24.24.24"
            << "United States"
            << "New York"
            << "Baldwinsville"
            << "AS11351 Road Runner HoldCo LLC"
            << "America/New_York"
            << (float) 43.1765
            << (float) -76.364
            << "US"
            << "NA";

    QTest::newRow("Hasselt, Belgium")
            << "84.193.172.100"
            << "Belgium"
            << "Limburg"
            << "Hasselt"
            << "AS6848 Telenet N.V."
            << "Europe/Brussels"
            << (float) 50.9333
            << (float) 5.3333
            << "BE"
            << "EU";

    QTest::newRow("Netherlands")
            << "194.151.127.217"
            << "Netherlands"
            << "Zuid-Holland"
            << "Den Haag"
            << "AS286 KPN Internet Backbone"
            << "Europe/Amsterdam"
            << (float) 52.0833
            << (float) 4.3
            << "NL"
            << "EU";
}
