#include <QtNetwork/QLocalSocket>

#include "singleinstance.h"

SingleInstance::SingleInstance(const QString& name, QObject* parent)
    : QObject(parent), mName(name)
{
    connect(&mServer, &QLocalServer::newConnection, this, &SingleInstance::onNewConnection);
}

QString SingleInstance::getLastErrorString() const
{
    return mServer.errorString();
}

bool SingleInstance::start()
{
    mServer.removeServer(mName);
    return mServer.listen(mName);
}

bool SingleInstance::hasPreviousInstance()
{
    QLocalSocket socket;
    socket.connectToServer(mName);

    return socket.waitForConnected();
}

void SingleInstance::onNewConnection()
{
    emit newInstanceCreated();
}
