#include "transfer.h"

Transfer::Transfer(QTcpSocket* socket, QObject* parent)
    : QObject(parent), mFile(nullptr), mSocket(nullptr),
      mPacketSize(-1)
{
    mInfo = new TransferInfo(this, this);
    setSocket(socket);
    mHeaderSize = sizeof(PacketType) + sizeof(mPacketSize);
}

void Transfer::resume()
{
    
}

void Transfer::pause()
{
    
}

void Transfer::cancel()
{
    
}

void Transfer::onReadyRead()
{
    /*
     * Abaikan data yg diterima jika state transfer 'Cancelled' (Dibatalkan)
     */
    if (mInfo->getState() == TransferState::Cancelled)
        return;

    mBuff.append(mSocket->readAll());

    /*
     * Jika ukuran buffer sudah >= ukuran header, maka pda buffer tsb
     * terdapat data header yg dpt kita extrak
     * dt header --> packet size (4 bytes) & packet type (1 byte)
     */
    while (mBuff.size() >= mHeaderSize) {
        if (mPacketSize < 0) {
            memcpy(&mPacketSize, mBuff.constData(), sizeof(mPacketSize));
            mBuff.remove(0, sizeof(mPacketSize));
        }

        if (mBuff.size() > mPacketSize) {
            PacketType type = static_cast<PacketType>(mBuff.at(0));
            QByteArray data = mBuff.mid(1, mPacketSize);

            processPacket(data, type);
            mBuff.remove(0, mPacketSize + 1);

            mPacketSize = -1;
        }
        else {
            break;
        }
    }
}

void Transfer::writePacket(qint32 packetDataSize, PacketType type, const QByteArray &data)
{
    if (mSocket) {
        mSocket->write(reinterpret_cast<const char*>(&packetDataSize), sizeof(packetDataSize));
        mSocket->write(reinterpret_cast<const char*>(&type), sizeof(type));
        mSocket->write(data);
    }
}

void Transfer::processPacket(QByteArray &data, PacketType type)
{
    switch (type) {
    case PacketType::Header : processHeaderPacket(data); break;
    case PacketType::Data : processDataPacket(data); break;
    case PacketType::Finish : processFinishPacket(data); break;
    case PacketType::Cancel : processCancelPacket(data); break;
    case PacketType::Pause : processPausePacket(data); break;
    case PacketType::Resume : processResumePacket(data); break;
    }
}

void Transfer::processHeaderPacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processDataPacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processFinishPacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processCancelPacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processPausePacket(QByteArray& data)
{
    Q_UNUSED(data);
}

void Transfer::processResumePacket(QByteArray& data)
{
    Q_UNUSED(data);
}


void Transfer::clearReadBuffer()
{
    mBuff.clear();
    mPacketSize = -1;
}

void Transfer::setSocket(QTcpSocket *socket)
{
    if (socket) {
        mSocket = socket;
        connect(mSocket, &QTcpSocket::readyRead, this, &Transfer::onReadyRead);
    }
}

