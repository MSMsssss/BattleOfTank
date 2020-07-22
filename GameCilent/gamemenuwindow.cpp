#include "gamemenuwindow.h"
#include "ui_gamemenuwindow.h"
#include "config.h"
#include "gameroom.h"
#include <QStandardItemModel>
#include <QList>
#include <QStandardItem>
#include <QStringList>
#include <QByteArray>
#include <QDataStream>

GameMenuWindow::GameMenuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameMenuWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800, 600);

    roomsModel = new QStandardItemModel(this);
    ui->roomsTableView->setModel(roomsModel);
    ui->roomsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->roomsTableView->setSelectionMode(QTableView::SingleSelection);
    ui->roomsTableView->setSelectionBehavior(QTableView::SelectRows);
    ui->roomsTableView->setEditTriggers(QTableView::NoEditTriggers);

    ui->refreshRoomInfoButton->setFixedWidth(88);
    ui->joinGamePushButton->setFixedWidth(88);
    connect(ui->refreshRoomInfoButton, &QPushButton::clicked, this, &GameMenuWindow::refreshRoomsInfo);
    connect(ui->joinGamePushButton, &QPushButton::clicked, this, &GameMenuWindow::joinGame);
}

GameMenuWindow::~GameMenuWindow()
{
    delete ui;
}

QString GameMenuWindow::selfName() const
{
    return _selfName;
}

void GameMenuWindow::setSelfName(const QString &selfName)
{
    _selfName = selfName;
}

void GameMenuWindow::handlePacket(QByteArray packet)
{
    quint16 type;
    QDataStream in(&packet, QIODevice::ReadOnly);
    in >> type;
    if(type == Config::ReplyRoomsInfoRequestPacket)
    {
        roomsModel->clear();

        QStringList columnLabels;
        columnLabels << QStringLiteral("房间名称")
                     << QStringLiteral("游玩的地图")
                     << QStringLiteral("游玩的模式")
                     << QStringLiteral("人数");
        roomsModel->setHorizontalHeaderLabels(columnLabels);

        int roomNum;
        in >> roomNum;

        for(int i = 0; i < roomNum; ++i)
        {
            GameRoom room;
            in >> room;

            QList<QStandardItem* > roomItemList;
            roomItemList << new QStandardItem(room.getRoomName())
                         << new QStandardItem(GameMap::getName(room.getGameMap()))
                         << new QStandardItem(GameMode::getName(room.getGameMode()))
                         << new QStandardItem(QString("%1 / %2").arg(room.getPlayerNum()).arg(room.getPlayerNumLimit()));

            roomsModel->appendRow(roomItemList);

            for(int j = 0; j < roomsModel->columnCount(); j++)
            {
                roomsModel->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }

            ui->refreshRoomInfoButton->setDisabled(false);
        }
    }
    else if (type == Config::ReplyJoinRoomPacket) {
        int status;
        QString roomName;
        in >> status;

        switch (status) {
        case Config::JoinSuccess:
            in >> roomName;
            emit joinRoomSuccess(roomName);
            break;
        case Config::RoomIsFull:
            ui->messageLabel->setText(QStringLiteral("房间已满！"));
            break;
        case Config::RepeatJoin:
            ui->messageLabel->setText(QStringLiteral("重复加入！"));
            break;
        case Config::RoomNotExists:
            ui->messageLabel->setText(QStringLiteral("房间不存在！"));
            break;
        }

        ui->joinGamePushButton->setEnabled(true);
    }
}

// 从服务器重新获取房间信息
void GameMenuWindow::refreshRoomsInfo()
{
    ui->refreshRoomInfoButton->setDisabled(true);
    QByteArray packet;
    QDataStream(&packet, QIODevice::WriteOnly) << Config::RoomsInfoRequestPacket;
    emit packetToSend(packet);
}

void GameMenuWindow::joinGame()
{
    ui->joinGamePushButton->setDisabled(true);
    ui->messageLabel->setText(QStringLiteral("加入游戏中..."));

    int row = ui->roomsTableView->currentIndex().row();
    QString roomName = roomsModel->item(row, 0)->text();

    QByteArray packet;
    QDataStream (&packet, QIODevice::WriteOnly) << Config::RequestJoinRoomPacket << roomName;
    emit packetToSend(packet);
}
