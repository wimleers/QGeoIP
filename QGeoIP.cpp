#include "QGeoIP.h"

QGeoIP::QGeoIP() {
    this->db = NULL;
}

QGeoIP::~QGeoIP() {
    this->close();
}

/**
 * Open a GeoIP database. If a database is already opened in the current
 * instance, it will be closed before the requested database is opened.
 *
 * @return
 *   true if successful, otherwise false.
 */
bool QGeoIP::open(const QString & fileName) {
    if (this->isOpen())
        this->close();

    this->db = GeoIP_open(fileName.toLocal8Bit().constData(), GEOIP_INDEX_CACHE);
    if (this->db) {
        GeoIP_set_charset(this->db, GEOIP_CHARSET_UTF8);
        return true;
    }
    else {
        qCritical("Unable to open MaxMind GeoIP database for reading: %s.", qPrintable(fileName));
        return false;
    }
}

void QGeoIP::close() {
    if (this->isOpen()) {
        GeoIP_delete(this->db);
        this->db = NULL;
    }
}

bool QGeoIP::isOpen() const {
  return this->db != NULL;
}

QGeoIP::DBType QGeoIP::type() const {
  if (!this->isOpen())
      return DBTypeUnknown;

  switch (this->db->databaseType) {
    case GEOIP_COUNTRY_EDITION:
    case GEOIP_COUNTRY_EDITION_V6:
      return DBTypeCountry;
    case GEOIP_CITY_EDITION_REV0:
    case GEOIP_CITY_EDITION_REV1:
      return DBTypeCity;
    case GEOIP_REGION_EDITION_REV0:
    case GEOIP_REGION_EDITION_REV1:
      return DBTypeRegion;
    case GEOIP_ORG_EDITION:
      return DBTypeOrganization;
    case GEOIP_ISP_EDITION:
      return DBTypeISP;
    case GEOIP_PROXY_EDITION:
      return DBTypeProxy;
    case GEOIP_ASNUM_EDITION:
      return DBTypeASN;
    case GEOIP_NETSPEED_EDITION:
      return DBTypeNetSpeed;
    case GEOIP_DOMAIN_EDITION:
      return DBTypeDomain;
    default:
      return DBTypeUnknown;
  }
}

/**
 * Resolve an IP address to the corresponding country code (ISO-3166-1-alpha2).
 *
 * @return
 *   The corresponding country code, or a null QString.
 */
QString QGeoIP::countryCodeByAddr(const QHostAddress & ip) {
    if (this->isOpen() && !ip.isNull()) {
        const char * addr = ip.toString().toAscii().constData();
        const char * countryCode = GeoIP_country_code_by_addr(this->db, addr);
        if (countryCode != NULL)
            return QString::fromUtf8(countryCode);
    }

    return QString::null;
}

/**
 * Resolve an IP address to a QGeoIPRecord, in which the following information
 * is contained:
 * - the country code (ISO-3166-1-alpha2)
 * - the country name
 * - city name
 * - region name
 * - latitude and longitude
 *
 * @return
 *   The corresponding QGeoIPRecord.
 */
QGeoIPRecord QGeoIP::recordByAddr(const QHostAddress & ip) {
    static QString country, city, region, isp, timeZone;
    static const char * addr, * regionName;

    if (this->isOpen() && !ip.isNull()) {
        addr = ip.toString().toAscii().constData();

        GeoIPRecord * r;
        if (ip.protocol() == QAbstractSocket::IPv6Protocol)
            r = GeoIP_record_by_addr_v6(this->db, addr);
        else
            r = GeoIP_record_by_addr(this->db, addr);

        // TODO: get ISP
        // GeoIP_org_by_addr
        // GeoIP_org_by_addr_v6

        timeZone = QString::fromUtf8(GeoIP_time_zone_by_country_and_region(r->country_code, r->region));

        if (r != NULL) {
            country = QString::fromUtf8(r->country_name);
            city = QString::fromUtf8(r->city);

            regionName = GeoIP_region_name_by_code(r->country_code, r->region);
            if (regionName != NULL)
                region = QString::fromUtf8(regionName);

            return QGeoIPRecord(country, region, city, "", timeZone, r->latitude, r->longitude, r->country_code, r->continent_code);
        }
    }

    return QGeoIPRecord();
}

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const QGeoIPRecord & record) {
    dbg.nospace() << record.country
                  << " ("
                  << record.countryCode
                  << ", "
                  << record.continentCode
                  << "), "
                  << record.region
                  << ", "
                  << record.city
                  << ", "
                  << record.isp
                  << ", "
                  << record.timeZone
                  << ", ("
                  << record.lat
                  << ","
                  << record.lon
                  << ")";

    return dbg.nospace();
}
#endif
