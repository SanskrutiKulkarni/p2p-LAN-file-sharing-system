#include "devicebroadcaster.h"
#include "settings.h"

#include <cassert>

DeviceBroadcaster::DeviceBroadcaster(QObject *parent) : QObject(parent)
{
    connect(&mTimer, &QTimer::timeout, this, &DeviceBroadcaster::sendBroadcast);
    connect(&mUdpSock, &QUdpSocket::readyRead, this, &DeviceBroadcaster::processBroadcast);

    mUdpSock.bind(Settings::instance()->getBroadcastPort(), QUdpSocket::ShareAddress);
}

void DeviceBroadcaster::start()
{
    sendBroadcast();
    if (!mTimer.isActive())
        mTimer.start(Settings::instance()->getBroadcastInterval());
}

void DeviceBroadcaster::sendBroadcast()
{
    int port = Settings::instance()->getBroadcastPort();
    Device dev = Settings::instance()->getMyDevice();
    QJsonObject obj(QJsonObject::fromVariantMap({
                                                    {"id", dev.getId()},
                                                    {"name", dev.getName()},
                                                    {"os", dev.getOSName()},
                                                    {"port", port}
                                                }));

    QVector<QHostAddress> addresses = getBroadcastAddressFromInterfaces();
    QByteArray data(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    foreach (QHostAddress address, addresses) {
        mUdpSock.writeDatagram(data, address, port);
    }
}

/*
 * proses broadcast yang diterima dari Device lain di jaringan
 */
void DeviceBroadcaster::processBroadcast()
{
    while (mUdpSock.hasPendingDatagrams()) {
        QByteArray data;
        
        qint64 datagramSize = mUdpSock.pendingDatagramSize();
        assert(datagramSize <= std::numeric_limits <int>::max());

        data.resize(static_cast<int>(datagramSize));
        QHostAddress sender;

        mUdpSock.readDatagram(data.data(), data.size(), &sender);

        QJsonObject obj = QJsonDocument::fromJson(data).object();
        if (obj.keys().length() == 4) {
            if (obj.value("port").toVariant().value<quint16>() ==
                    Settings::instance()->getBroadcastPort()) {

                Device device{obj.value("id").toString(), obj.value("name").toString(),
                              obj.value("os").toString(), sender};
                emit broadcastReceived(device);
            }
        }
    }
}

QVector<QHostAddress> DeviceBroadcaster::getBroadcastAddressFromInterfaces()
{
    QVector<QHostAddress> addresses;
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces()) {
        if (iface.flags() & QNetworkInterface::CanBroadcast) {
            foreach (QNetworkAddressEntry addressEntry, iface.addressEntries()) {
                if (!addressEntry.broadcast().isNull()) {
                    addresses.push_back(addressEntry.broadcast());
                }
            }
        }
    }
    return addresses;
}
