#include <QNetworkInterface>
#include <QHostInfo>
#include <QUuid>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>

#include "settings.h"

#define DefaultBroadcastPort        56780
#define DefaultTransferPort         17116
#define DefaultBroadcastInterval    5000    // 5 secs
#define DefaultFileBufferSize       98304   // 96 KB
#define MaxFileBufferSize           1024*1024 // 1 MB

Settings* Settings::obj = new Settings;
Settings::Settings()
{
    mThisDevice = Device();
    mThisDevice.setId(QUuid::createUuid().toString());
    mThisDevice.setAddress(QHostAddress::LocalHost);
    mThisDevice.setOSName(OS_NAME);

    foreach (QHostAddress address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::LocalHost) {
            mThisDevice.setAddress(address);
            break;
        }
    }

    loadSettings();
}

void Settings::setDeviceName(const QString &name)
{
    mThisDevice.setName(name);
}

void Settings::setBroadcastPort(quint16 port)
{
    if (port > 0)
        mBCPort = port;
}

void Settings::setBroadcastInterval(quint16 interval)
{
    mBCInterval = interval;
}

void Settings::setTransferPort(quint16 port)
{
    if (port > 0)
        mTransferPort = port;
}

void Settings::setFileBufferSize(qint32 size)
{
    if (size > 0 && size < MaxFileBufferSize)
        mFileBuffSize = size;
}

void Settings::setDownloadDir(const QString& dir)
{
    if (!dir.isEmpty() && QDir(dir).exists())
        mDownloadDir = dir;
}

void Settings::setReplaceExistingFile(bool replace)
{
    mReplaceExistingFile = replace;
}

void Settings::loadSettings()
{
    QSettings settings(SETTINGS_FILE);
    mThisDevice.setName(settings.value("DeviceName", QHostInfo::localHostName()).toString());
    mBCPort = settings.value("BroadcastPort", DefaultBroadcastPort).value<quint16>();
    mTransferPort = settings.value("TransferPort", DefaultTransferPort).value<quint16>();
    mFileBuffSize = settings.value("FileBufferSize", DefaultFileBufferSize).value<quint32>();
    mDownloadDir = settings.value("DownloadDir", getDefaultDownloadPath()).toString();

    if (!QDir(mDownloadDir).exists()) {
        QDir dir;
        dir.mkpath(mDownloadDir);
    }

    mBCInterval = settings.value("BroadcastInterval", DefaultBroadcastInterval).value<quint16>();
    mReplaceExistingFile = settings.value("ReplaceExistingFile", false).toBool();
}

QString Settings::getDefaultDownloadPath()
{
#if defined (Q_OS_WIN)
    return
        QStandardPaths::locate(QStandardPaths::DownloadLocation, QString(), QStandardPaths::LocateDirectory) + "LANShareDownloads";
#else
    return QDir::homePath() + QDir::separator() + "LANShareDownloads";
#endif
}

void Settings::saveSettings()
{
    QSettings settings(SETTINGS_FILE);
    settings.setValue("DeviceName", mThisDevice.getName());
    settings.setValue("BroadcastPort", mBCPort);
    settings.setValue("TransferPort", mTransferPort);
    settings.setValue("FileBufferSize", mFileBuffSize);
    settings.setValue("DownloadDir", mDownloadDir);
    settings.setValue("BroadcastInterval", mBCInterval);
    settings.setValue("ReplaceExistingFile", mReplaceExistingFile);
}

void Settings::reset()
{
    mThisDevice.setName(QHostInfo::localHostName());
    mBCPort = DefaultBroadcastPort;
    mTransferPort = DefaultTransferPort;
    mBCInterval = DefaultBroadcastInterval;
    mFileBuffSize = DefaultFileBufferSize;
    mDownloadDir = getDefaultDownloadPath();
}

quint16 Settings::getBroadcastPort() const
{
    return mBCPort; 
}

quint16 Settings::getTransferPort() const
{
    return mTransferPort;
}

quint16 Settings::getBroadcastInterval() const
{
    return mBCInterval;
}

qint32 Settings::getFileBufferSize() const 
{ 
    return mFileBuffSize; 
}

QString Settings::getDownloadDir() const
{
    return mDownloadDir;
}

Device Settings::getMyDevice() const
{
    return mThisDevice;
}

QString Settings::getDeviceId() const
{
    return mThisDevice.getId();
}

QString Settings::getDeviceName() const
{
    return mThisDevice.getName();
}

QHostAddress Settings::getDeviceAddress() const
{
    return mThisDevice.getAddress();
}

bool Settings::getReplaceExistingFile() const
{
    return mReplaceExistingFile;
}

