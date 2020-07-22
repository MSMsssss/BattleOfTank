#ifndef THREADSSERVER_H
#define THREADSSERVER_H

#include <QTcpServer>
#include <QHash>
class PacketTcpSocket;

class ThreadsServer : public QTcpServer
{
    Q_OBJECT

private:
    // 玩家姓名至套接字的映射
    QHash<QString, PacketTcpSocket*> nameToSocket;
    QHash<PacketTcpSocket*, QString> socketToName;

public:
    explicit ThreadsServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr handle) override;
    int handleLogin(const QString& playerName, const QString& password);

signals:
    //  用于连接所建立的套接字，并对其分发需要发送给客户端的数据包
    void packetToSend(PacketTcpSocket* socket, QByteArray packet);

    //  用于连接需要客户端发送的数据包的模块
    void newPacket(QString playerName, QByteArray packet);

    // 玩家登录
    void playerLoginIn(QString playerName);

    //  玩家退出
    void playerLoginOut(QString playerName);

public slots:
    //  threads server 用于处理到来数据包的槽函数
    void handlePacket(QByteArray packet);

    //  用于给需要发送数据包的模块提供接口
    void sendPacket(QString playerName, QByteArray packet);

    //  玩家登出后所需要的一些处理
    void loginOut();
};

#endif // THREADSSERVER_H
