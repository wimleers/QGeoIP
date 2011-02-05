#ifndef QGEOIP_H
#define QGEOIP_H


#include <QString>
#include <QHostAddress>
#include <QMetaType>

#include <GeoIP.h>
#include <GeoIPCity.h>

#ifdef DEBUG
#include <QDebug>
#endif


struct QGeoIPRecord {
    QGeoIPRecord() { countryCode[0] = NULL; }
    QGeoIPRecord(QString country, QString region, QString city, QString isp, QString timeZone, float lat, float lon, char * countryCode, char * continentCode)
     : country(country), region(region), city(city), isp(isp), timeZone(timeZone), lat(lat), lon(lon)
    {
        // Minimize memory consumption.
        this->country.squeeze();
        this->region.squeeze();
        this->city.squeeze();
        this->isp.squeeze();
        this->timeZone.squeeze();

        strcpy(this->countryCode, countryCode);
        strcpy(this->continentCode, continentCode);
    }

    QString country;  // After these QStrings have been filled with their
    QString region;   // final values, they should be squeezed
    QString city;     // (QString::squeeze()), to minimize their memory
    QString isp;      // consumption.
    QString timeZone; //
    float lat;
    float lon;
    char countryCode[3]; // Stores a ISO-3166-1-alpha2 country code.
    char continentCode[3];
};

// Register metatype to allow these types to be streamed in QTests.
Q_DECLARE_METATYPE(QGeoIPRecord)

#ifdef DEBUG
// QDebug() streaming output operators.
QDebug operator<<(QDebug dbg, const QGeoIPRecord & record);
#endif




class QGeoIP {
public:
    enum DBType {
      DBTypeUnknown = 0,
      DBTypeCountry,
      DBTypeCity,
      DBTypeRegion,
      DBTypeOrganization,
      DBTypeISP,
      DBTypeProxy,
      DBTypeASN,
      DBTypeNetSpeed,
      DBTypeDomain
    };

    QGeoIP();
    ~QGeoIP();

    bool openDatabases(const QString & cityDBFile, const QString & ISPDBFile);
    void closeDatabases();

    QGeoIPRecord recordByAddr(const QHostAddress & ip);

private:
    bool open(GeoIP ** db, const QString & fileName);
    void close(GeoIP ** db);
    bool isOpen(GeoIP const * const db) const;
    QGeoIP::DBType type() const;

    GeoIP * cityDB;
    GeoIP * ISPDB;
};

#endif // QGEOIP_H
