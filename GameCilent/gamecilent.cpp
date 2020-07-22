#include "gamecilent.h"
#include "function.h"
#include "config.h"
#include "packettcpsocket.h"
#include "logindialog.h"
#include "gamemenuwindow.h"
#include "gameplayingwindow.h"
#include <QLabel>
#include <QThread>
#include <QMessageBox>
#include <QApplication>

GameCilent::GameCilent(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(600, 300);
    QLabel* message = new QLabel(this);
    Function::initLabel(message);
    message->setText(QStringLiteral("连接服务器中..."));
    message->move((this->width() - message->width()) / 2,
                  (this->height() - message->height()) / 2);

    socket = new PacketTcpSocket();
    socketThread = new QThread();

    connect(socket, &PacketTcpSocket::disconnected, socketThread, &QThread::quit);
    connect(socket, &PacketTcpSocket::disconnected, this, &GameCilent::handleSocketError);
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(handleSocketError()));
    connect(socket, &PacketTcpSocket::connected, this, &GameCilent::handleSocketConnected);

    connect(socketThread, &QThread::finished, socketThread, &QThread::deleteLater);
    connect(socketThread, &QThread::finished, socket, &PacketTcpSocket::deleteLater);
}

GameCilent::~GameCilent()
{
}

int GameCilent::run()
{
    if(Config::TestMode)
    {
        qDebug() << "Main thread: " << QThread::currentThreadId();
    }

//    gameMenuWindow = new GameMenuWindow(this);
//    gameMenuWindow->show();

    this->show();
    socket->connectToHost(Config::serverAddress, Config::serverPort);

    return app->exec();
}

void GameCilent::handleSocketError()
{
    QMessageBox::critical(this, QStringLiteral("错误"),
                          QStringLiteral("连接服务器失败，请检查网络连接情况。"),
                          QMessageBox::Yes, QMessageBox::Yes);

    app->quit();
}

void GameCilent::handleSocketConnected()
{
    socket->moveToThread(socketThread);
    socketThread->start();

    this->close();
    loginDialog = new LoginDialog(this);

    connect(socket, SIGNAL(newPacket(QByteArray)), loginDialog, SLOT(handlePacket(QByteArray)));
    connect(loginDialog, SIGNAL(packetToSend(QByteArray)), socket, SLOT(sendPacket(QByteArray)));
    connect(loginDialog, SIGNAL(loginSuccess(QString)), this, SLOT(handleLoginSuccess(QString)));

    loginDialog->show();
}

void GameCilent::handleLoginSuccess(QString selfName)
{
    loginDialog->close();
    loginDialog->deleteLater();

    gameMenuWindow = new GameMenuWindow(this);
    gameMenuWindow->setSelfName(selfName);

    gamePlayingWindow = new GamePlayingWindow(this);
    gamePlayingWindow->setSelfName(selfName);

    connect(socket, SIGNAL(newPacket(QByteArray)), gameMenuWindow, SLOT(handlePacket(QByteArray)));
    connect(gameMenuWindow, SIGNAL(packetToSend(QByteArray)), socket, SLOT(sendPacket(QByteArray)));
    connect(gameMenuWindow, SIGNAL(joinRoomSuccess(QString)), this, SLOT(handleJoinRoom(QString)));

    connect(socket, SIGNAL(newPacket(QByteArray)), gamePlayingWindow, SLOT(handlePacket(QByteArray)));
    connect(gamePlayingWindow, SIGNAL(packetToSend(QByteArray)), socket, SLOT(sendPacket(QByteArray)));

    gameMenuWindow->show();
}

void GameCilent::handleJoinRoom(QString roomName)
{
    gameMenuWindow->close();
    gameMenuWindow->deleteLater();

    gamePlayingWindow->setRoomName(roomName);
    gamePlayingWindow->startGame();
    gamePlayingWindow->show();
    gamePlayingWindow->setFocus();
}
