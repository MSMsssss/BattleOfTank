#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QApplication>
#include <QHash>

class ThreadsServer;
class GameRoom;

class GameServer : public QObject
{
    Q_OBJECT

public:
    enum PlayerStatus{
        GamePlaying,    //游戏游玩中
        StayInMenu  //  停留在菜单界面
    };

public:
    explicit GameServer(QObject *parent = nullptr);
    ~GameServer();
    int run();

private:
    void initRooms();

public:
    QApplication *app;

private:
    ThreadsServer* threadsServer;
    QHash<QString, GameRoom*> roomHash; //房间名 -> 房间
    QHash<QString, PlayerStatus> playerStatus;  //玩家姓名 -> 当前状态
    QHash<QString, GameRoom*> playerToRoom;   // 游戏中玩家姓名  ->  所在房间

signals:
    // 向客户端发送数据
    void packetToSend(QString playerName, QByteArray packet);

    //  向服务器内其他模块发送数据
    void newPacket(QString playerName, QByteArray packet);

public slots:
    //
    void handlePacketFromCilent(QString playerName, QByteArray packet);
    void handlePacketFromGameRunning(QString playerName, QByteArray packet);
    void handleLoginIn(QString playerName);
    void handleLoginOut(QString playerName);
};

#endif // GAMESERVER_H
