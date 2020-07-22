#ifndef GAMEPLAYINGWINDOW_H
#define GAMEPLAYINGWINDOW_H

#include <QMainWindow>
#include <QQueue>
#include <QVector>
#include <QList>
#include <QSet>
#include <QHash>
#include <QPoint>
#include <QUrl>
#include <QTime>
#include "model/playercontroller.h"
#include "model/blockactor.h"
#include "model/projectileactor.h"
#include "model/tankactor.h"
#include "gamesound.h"

class QLabel;

inline uint qHash(const QPoint& key)
{
    return qHash(key.x()) ^ qHash(key.y());
}

namespace Ui {
class GamePlayingWindow;
}

class GamePlayingWindow : public QMainWindow
{
    Q_OBJECT

public:
    class ParticleSystem{
    public:
        using ParticleType = int;
        static const ParticleType Smoke = 1;
        static const ParticleType Bomb = 2;
        static const ParticleType Fire = 3;
        static const ParticleType TankBomb = 4;
    };

    explicit GamePlayingWindow(QWidget *parent = nullptr);
    ~GamePlayingWindow();

    QString getRoomName() const;
    void setRoomName(const QString &value);

    QString selfName() const;
    void setSelfName(const QString &selfName);

private:
    Ui::GamePlayingWindow *ui;
    QString roomName;
    QString _selfName;
    QSize _mapSize = {0, 0};
    QVector<BlockActor> boundaryWall;
    int wallThick = 20; //墙厚度

    QImage* blueCompanyTankSource;
    QImage* redCompanyTankSource;
    QImage* armorBulletSource;
    QImage* machineGunBulletSource;
    QImage* blockSource;

    QLabel* screenShowLabel;
    QSize _cameraSize;

    QTimer* computeTimer = nullptr;
    QTimer* printTimer = nullptr;
    QTimer* killTipsTimer = nullptr;
    QTimer* machineGunTimer = nullptr;

    int killMessageFlushInterval = 1000;
    int killTipsShowInterval = 4000;
    int computeInterval = 60; //msec，逻辑帧间隔
    int printInterval = 20; //msec，输出帧间隔
    QTime lastClickTime;
    int moveKeyPress;
    PlayerController selfController;

    QVector<QImage> frameBuffer; //  帧生成缓冲区
    QVector<PlayerController> playersSet;  //玩家集合
    QVector<ProjectileActor> projectileActorSet;    //子弹Actor集合
    QHash<QPoint, BlockActor> blockActorHash; //墙体Actor集合

    static const int DataBufferSize = 3;
    static const int PlayerInfoPacketIndex = 0;
    static const int ProjectileInfoPacketIndex = 1;
    static const int BlockInfoPacketIndex = 2;
    QVector<QByteArray> dataBuffer;

    //  击杀消息
    QVector<QPair<QString, QString>> killMessage;   // first kill second
    QList<QPair<QString, QString>> killMessageToShow;
    int maxKillMessageToShow = 5;

    //  爆炸队列
    QVector<QPair<QPoint, ParticleSystem::ParticleType>> particleMessage;    //  发射地点和粒子类型

signals:
    void packetToSend(QByteArray packet);
    void playSound(GameSound::SoundType type, int loopCount, bool isInstant);
    void stopSound(GameSound::SoundType type);
    void quitGame();

public slots:
    void compute();
    void print();
    void startGame();
    void handlePacket(QByteArray packet);

protected slots:
    void flushKillMessage();
    void setKillTipsVisible(bool visible);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // GAMEPLAYINGWINDOW_H
