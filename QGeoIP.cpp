#include "QGeoIP.h"

QGeoIP::QGeoIP() {
    this->cityDB = NULL;
    this->ISPDB  = NULL;
}

QGeoIP::~QGeoIP() {
    this->closeDatabases();
}

bool QGeoIP::openDatabases(const QString & cityDBFile, const QString & ISPDBFile) {
    if (!this->open(&this->cityDB, cityDBFile)) {
        qCritical("Could not open city DB, %s.", qPrintable(cityDBFile));
        return false;
    }
    if (!this->open(&this->ISPDB, ISPDBFile)) {
        qCritical("Could not open ISP DB, %s.", qPrintable(ISPDBFile));
        return false;
    }

    return true;
}

void QGeoIP::closeDatabases() {
    this->close(&this->cityDB);
    this->close(&this->ISPDB);
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
    const char * addr, * regionName, * ispName;

    if (this->isOpen(this->cityDB) && !ip.isNull()) {
        addr = ip.toString().toAscii().constData();

        GeoIPRecord * r;
        if (ip.protocol() == QAbstractSocket::IPv6Protocol)
            r = GeoIP_record_by_addr_v6(this->cityDB, addr);
        else
            r = GeoIP_record_by_addr(this->cityDB, addr);

        // Look up the ISP for this IP address, but only if an ISP DB is open.
        isp = QString::null;
        if (this->isOpen(this->ISPDB)) {
            if (ip.protocol() == QAbstractSocket::IPv6Protocol)
                ispName = GeoIP_org_by_name_v6(this->ISPDB, addr);
            else
                ispName = GeoIP_org_by_name(this->ISPDB, addr);

            if (ispName != NULL) {
                isp = QString::fromUtf8(ispName);
                free((char *) ispName);
            }
        }
        // Since addr really is an alias that points directly the the data
        // stored by the QHostAddress data structure, it doesn't need to be
        // deleted.

        timeZone = QString::fromUtf8(GeoIP_time_zone_by_country_and_region(r->country_code, r->region));

        if (r != NULL) {
            country = QString::fromUtf8(r->country_name);
            city = QString::fromUtf8(r->city);

            regionName = GeoIP_region_name_by_code(r->country_code, r->region);
            if (regionName != NULL) {
                region = QString::fromUtf8(regionName);
                // No free() call required for regionName, since it is actually
                // pointing to a hardcoded string in libGeoIP's C code.
            }

            // Create the result that will be returned to the user.
            QGeoIPRecord result(country, region, city, isp, timeZone, r->latitude, r->longitude, r->country_code, r->continent_code);

            // Delete the GeoIPRecord returned by GeoIP_record_by_addr() from
            // memory now that it is no longer needed.
            GeoIPRecord_delete(r);

            // Return the result we found.
            return result;
        }
    }

    // This function should only be called when at least the City DB is open.
    qWarning("QGeoIP::recordByAddr(): MaxMind City DB is not open.");

    // Satisfy compiler.
    return QGeoIPRecord();
}


//-----------------------------------------------------------------------------------
// Private methods.

/**
 * Open a GeoIP database. If a database is already opened in the current
 * instance, it will be closed before the requested database is opened.
 *
 * @return
 *   true if successful, otherwise false.
 */
bool QGeoIP::open(GeoIP ** db, const QString & fileName) {
    *db = GeoIP_open(fileName.toLocal8Bit().constData(), GEOIP_MMAP_CACHE);
    if (this->isOpen(*db)) {
        GeoIP_set_charset(*db, GEOIP_CHARSET_UTF8);
        return true;
    }
    else
        return false;
}

void QGeoIP::close(GeoIP ** db) {
    if (this->isOpen(*db)) {
        GeoIP_delete(*db);
        *db = NULL;
    }
}

bool QGeoIP::isOpen(GeoIP const * const db) const {
    return db != NULL;
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
