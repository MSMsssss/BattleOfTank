#include "packettcpsocket.h"
#include "config.h"
#include <QByteArray>
#include <QDataStream>
#include <QThread>
#include <QDebug>

PacketTcpSocket::PacketTcpSocket(QObject* parent): QTcpSocket(parent)
{
    packetSize = 0;
    connect(this, &PacketTcpSocket::readyRead, this, &PacketTcpSocket::readDataFromSocket);
}

PacketTcpSocket::~PacketTcpSocket()
{

}

/* 重置套接字 */
void PacketTcpSocket::resetSocket()
{
    this->abort();
    packetSize = 0;
}

void PacketTcpSocket::_sendPacket(const QByteArray &packet)
{
    QByteArray finalPacket;
    QDataStream(&finalPacket, QIODevice::WriteOnly) << (quint16)(packet.size());
    finalPacket = finalPacket + packet;

    this->write(finalPacket);
    this->flush();
}

/* 发送一个Packet */
void PacketTcpSocket::sendPacket(QByteArray packet)
{
//    if(Config::TestMode)
//    {
//        qDebug() << "socket write thread: " << QThread::currentThreadId();
//    }

    _sendPacket(packet);
}

// 当套接字发出readyRead信号时，该slot函数试图从套接字中读取一个packet
void PacketTcpSocket::readDataFromSocket()
{
//    if(Config::TestMode)
//    {
//        qDebug() << "socket read thread: " << QThread::currentThreadId();
//    }
    while(true)
    {
        if(packetSize == 0)
        {
            QDataStream in(this);
            in.setVersion(QDataStream::Qt_5_15);

            //判断是否已接受到数据包的长度
            if(this->bytesAvailable() < (int)sizeof (quint16)) return;
            //获得数据包的长度
            in >> packetSize;
        }

        //未获得全部数据则返回
        if(this->bytesAvailable() < packetSize)
            return;
        else
        {
            /*发送数据包*/
            QByteArray buf = this->read(packetSize);
            emit newPacket(buf);

            packetSize = 0;
        }
    }
}
