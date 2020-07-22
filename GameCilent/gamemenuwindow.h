#ifndef GAMEMENUWINDOW_H
#define GAMEMENUWINDOW_H

#include <QMainWindow>

class QStandardItemModel;
class QStandardItem;

QT_BEGIN_NAMESPACE
namespace Ui { class GameMenuWindow; }
QT_END_NAMESPACE

class GameMenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameMenuWindow(QWidget *parent = nullptr);
    ~GameMenuWindow();

    QString selfName() const;
    void setSelfName(const QString &selfName);

private:
    Ui::GameMenuWindow *ui;
    QStandardItemModel* roomsModel;
    QString _selfName;

signals:
    void packetToSend(QByteArray packet);
    void joinRoomSuccess(QString roomName);

public slots:
    void handlePacket(QByteArray packet);
    void refreshRoomsInfo();
    void joinGame();
};
#endif // GAMEMENUWINDOW_H
