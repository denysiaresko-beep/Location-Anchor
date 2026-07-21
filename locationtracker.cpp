#include "locationtracker.h"

LocationTracker::LocationTracker(QObject *parent): QObject(parent), isMarked(false), radiusThreshold(50.0), timeThresholdSeconds(5) {
    source = QGeoPositionInfoSource::createDefaultSource(this);

    if (source) {
        m_status = "Searching setelite...";
        source->setUpdateInterval(5000);
        connect(source, &QGeoPositionInfoSource::positionUpdated,
                this, &LocationTracker::onPositionUpdated);
        source->startUpdates();
    } else {
        m_status = "GPS is NOT found!!! (Use simulation instead)";
    }
}

void LocationTracker::simulateLocation(double lat, double lon) {
    QGeoPositionInfo info;
    info.setCoordinate(QGeoCoordinate(lat, lon));
    info.setTimestamp(QDateTime::currentDateTime());
    onPositionUpdated(info);
}

