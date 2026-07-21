#ifndef LOCATIONTRACKER_H
#define LOCATIONTRACKER_H
#include <QObject>
#include <QQmlEngine>
#include <QGeoPositionInfoSource>
#include <QGeoCoordinate>
#include <QDateTime>

class LocationTracker : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(double latitude READ latitude NOTIFY coordsChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY coordsChanged)

public:
    explicit LocationTracker(QObject *parent = nullptr);

    QString status() const { return m_status; }
    double latitude() const { return currentCoord.isValid() ? currentCoord.latitude() : 0.0; }
    double longitude() const { return currentCoord.isValid() ? currentCoord.longitude() : 0.0; }

    Q_INVOKABLE void simulateLocation(double lat, double lon);


signals:
    void statusChanged();
    void coordsChanged();
    void markCreated(double lat, double lon, QString timeString);

private slots:
    void onPositionUpdated(const QGeoPositionInfo &info) {
        currentCoord = info.coordinate();
        emit coordsChanged();

        if (!currentCoord.isValid()) return;

        if (!baseCoord.isValid()) {
            baseCoord = currentCoord;
            firstSeenTime = QDateTime::currentDateTime();
            m_status = "Scaning position...";
            emit statusChanged();
            return;
        }

        qreal distance = baseCoord.distanceTo(currentCoord);

        if (distance <= radiusThreshold) {
            if (!isMarked) {
                qint64 elapsedSeconds = firstSeenTime.secsTo(QDateTime::currentDateTime());
                if (elapsedSeconds >= timeThresholdSeconds) {
                    isMarked = true;
                    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss");
                    emit markCreated(currentCoord.latitude(), currentCoord.longitude(), timeStr);

                    m_status = "Note Created!";
                    emit statusChanged();
                }
            }
        } else {
            baseCoord = currentCoord;
            firstSeenTime = QDateTime::currentDateTime();
            isMarked = false;
            m_status = "Moving. Timer restarted.";
            emit statusChanged();
        }
    }

private:
    QGeoPositionInfoSource *source = nullptr;
    QGeoCoordinate baseCoord;
    QGeoCoordinate currentCoord;
    QDateTime firstSeenTime;

    QString m_status;
    bool isMarked;
    qreal radiusThreshold;
    qint64 timeThresholdSeconds;
};

#endif // LOCATIONTRACKER_H
