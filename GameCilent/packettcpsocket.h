#ifndef PACKETTCPSOCKET_H
#define PACKETTCPSOCKET_H
#include <QTcpSocket>

class PacketTcpSocket: public QTcpSocket
{

    Q_OBJECT

public:
    PacketTcpSocket(QObject* parent = nullptr);
    ~PacketTcpSocket();

    void resetSocket();

protected:
    quint16 packetSize;

protected:
    void _sendPacket(const QByteArray& packet);

signals:
    void newPacket(QByteArray packet);

public slots:
    void sendPacket(QByteArray packet);

protected slots:
    void readDataFromSocket();
};

#endif // PACKETTCPSOCKET_H
