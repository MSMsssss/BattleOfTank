#ifndef GAMECILENT_H
#define GAMECILENT_H

#include <QWidget>

class LoginDialog;
class GameMenuWindow;
class GamePlayingWindow;
class PacketTcpSocket;
class QApplication;

class GameCilent : public QWidget
{
    Q_OBJECT

public:
    QApplication* app;

private:
    LoginDialog* loginDialog;
    GameMenuWindow* gameMenuWindow;
    GamePlayingWindow* gamePlayingWindow;

    PacketTcpSocket* socket;
    QThread* socketThread;

public:
    explicit GameCilent(QWidget *parent = nullptr);
    ~GameCilent();
    int run();

public slots:
    void handleSocketError();
    void handleSocketConnected();
    void handleLoginSuccess(QString selfName);
    void handleJoinRoom(QString roomName);
    //void handlePacket(QByteArray);
};

#endif // GAMECILENT_H
