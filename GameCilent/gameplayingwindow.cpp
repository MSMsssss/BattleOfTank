#include "gameplayingwindow.h"
#include "ui_gameplayingwindow.h"
#include "config.h"
#include "playeroperation.h"
#include "model/gamecamera.h"
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QMatrix>
#include <QDataStream>
#include <QByteArray>
#include <QThread>
#include <QSoundEffect>

GamePlayingWindow::GamePlayingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GamePlayingWindow),
    _cameraSize(1280, 720)
{
    ui->setupUi(this);
    ui->healthProgressBar->setMinimum(0);
    ui->healthProgressBar->setMaximum(100);
    ui->healthProgressBar->setValue(100);

    setKillTipsVisible(false);

    ui->armorProgressBar->setMinimum(0);
    ui->armorProgressBar->setMaximum(100);
    ui->armorProgressBar->setValue(0);

    blueCompanyTankSource = new QImage("./picture/tank_12.png");
    redCompanyTankSource = new QImage("./picture/tank_11.png");
    armorBulletSource = new QImage("./picture/bullet_2.png");
    machineGunBulletSource = new QImage("./picture/bullet_1.png");
    blockSource = new QImage("./picture/block.png");


    screenShowLabel = new QLabel(this);
    screenShowLabel->resize(_cameraSize.width(), _cameraSize.height());
    screenShowLabel->move((this->width() - screenShowLabel->width()) / 2,
                (this->height() - screenShowLabel->height()) / 2);

    computeTimer = new QTimer(this);
    //computeTimer->setTimerType(Qt::PreciseTimer);
    connect(computeTimer, &QTimer::timeout, this, &GamePlayingWindow::compute);

    printTimer = new QTimer(this);
    //printTimer->setTimerType(Qt::PreciseTimer);
    connect(printTimer, &QTimer::timeout, this, &GamePlayingWindow::print);

    killTipsTimer = new QTimer(this);
    killTipsTimer->setInterval(killTipsShowInterval);
    connect(killTipsTimer, &QTimer::timeout, [this]()
    {
        this->setKillTipsVisible(false);
        this->killTipsTimer->stop();
    });

    QTimer* flushTimer = new QTimer(this);
    connect(flushTimer, &QTimer::timeout, this, &GamePlayingWindow::flushKillMessage);
    flushTimer->start(killMessageFlushInterval);

    machineGunTimer = new QTimer(this);
    machineGunTimer->setInterval(1200);
    connect(machineGunTimer, &QTimer::timeout, [this](){
        emit playSound(GameSound::MachineGunFireSound, QSoundEffect::Infinite, false);
    });

    lastClickTime = QTime::currentTime();

    dataBuffer.resize(GamePlayingWindow::DataBufferSize);
    dataBuffer[PlayerInfoPacketIndex].resize(0);
    dataBuffer[ProjectileInfoPacketIndex].resize(0);

    boundaryWall.resize(4);

    GameSound* gameSound = new GameSound();
    QThread* thread = new QThread();

    connect(this, SIGNAL(playSound(GameSound::SoundType, int, bool)),
            gameSound, SLOT(play(GameSound::SoundType, int, bool)));

    connect(this, SIGNAL(stopSound(GameSound::SoundType)), gameSound, SLOT(stop(GameSound::SoundType)));
    connect(this, &GamePlayingWindow::quitGame, thread, &QThread::quit);
    connect(thread, &QThread::finished, gameSound, &GameSound::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    gameSound->moveToThread(thread);
    thread->start();
}

GamePlayingWindow::~GamePlayingWindow()
{
    delete blueCompanyTankSource;
    delete redCompanyTankSource;
    delete armorBulletSource;
    delete machineGunBulletSource;
    delete blockSource;
    delete ui;
}

QString GamePlayingWindow::getRoomName() const
{
    return roomName;
}

void GamePlayingWindow::setRoomName(const QString &value)
{
    roomName = value;
}

QString GamePlayingWindow::selfName() const
{
    return _selfName;
}

void GamePlayingWindow::setSelfName(const QString &selfName)
{
    _selfName = selfName;
}

void GamePlayingWindow::compute()
{
    playersSet.clear();
    projectileActorSet.clear();

    if(!dataBuffer[PlayerInfoPacketIndex].isEmpty())
    {
        QDataStream in(&dataBuffer[PlayerInfoPacketIndex], QIODevice::ReadOnly);
        quint16 type;
        in >> type;

        int num = 0;
        in >> num;

        PlayerController controller;
        for(int i = 0; i < num; ++i)
        {
            in >> controller;
            if(controller.company() == PlayerController::BLUE_COMPANY)
                controller.tankRef().setSource(blueCompanyTankSource);
            else
                controller.tankRef().setSource(redCompanyTankSource);

            if(controller.playerName() == _selfName)
            {
                if(controller.isDead())
                {
                    QPoint temp = selfController.deathLocation();
                    selfController = controller;
                    selfController.setDeathLocation(temp);
                }
                else
                {
                    selfController = controller;
                }
            }

            playersSet.push_back(controller);
        }
    }

    if(!dataBuffer[ProjectileInfoPacketIndex].isEmpty())
    {
        QDataStream in(&dataBuffer[ProjectileInfoPacketIndex], QIODevice::ReadOnly);
        quint16 type;
        in >> type;

        int num = 0;
        in >> num;

        ProjectileActor bullet;
        for(int i = 0; i < num; ++i)
        {
            in >> bullet;
            if(bullet.type() == ProjectileActor::ArmorPiercingProjectile)
                bullet.setSource(armorBulletSource);
            else
                bullet.setSource(machineGunBulletSource);

            projectileActorSet.push_back(bullet);
        }
    }

    GameCamera camera(_cameraSize);
    int frameNum = computeInterval / printInterval;
    frameBuffer.resize(0);

    for(int i = 1; i <= frameNum; ++i)
    {
        QPoint tankLocation = {0, 0};
        //  设置相机位置
        if(!selfController.isDead())
        {
            Vector2D delta;
            if(i == frameNum)
                delta = selfController.tankPtr()->delta();
            else
                delta = selfController.tankPtr()->delta() * i / frameNum;

            tankLocation = selfController.tankPtr()->oldLocation() + delta;

        }
        else
        {
            tankLocation = selfController.deathLocation();
        }

        camera.setLocation({tankLocation.x() - (camera.width() - selfController.tankRefConst().width()) / 2,
                           tankLocation.y() - (camera.height() - selfController.tankRefConst().height()) / 2});


        //  显示子弹
        for(const ProjectileActor& bullet : projectileActorSet)
        {
            Vector2D delta;
            if(i == frameNum)
                delta = bullet.delta();
            else
                delta = bullet.delta() * i / frameNum;

            camera.showActor(bullet, delta);
        }

        //  显示坦克
        for(const PlayerController& controller : playersSet)
        {
            if(!controller.isDead())
            {
                Vector2D delta;
                if(i == frameNum)
                    delta = controller.tankPtr()->delta();
                else
                    delta = controller.tankPtr()->delta() * i / frameNum;

                camera.showActor(controller.tankRefConst(), delta);
            }
        }

        //  显示墙块
        for(const BlockActor& block : blockActorHash)
        {
            camera.showActor(block);
        }

        if(_mapSize.width() != 0)
        {
            //  显示边界
            for(const BlockActor& block : boundaryWall)
            {
                camera.showActor(block);
            }
        }

        frameBuffer.push_back(camera.cameraRef());
    }

    //处理击杀消息
    {
        for(const auto& killPair : killMessage)
        {
            if(killPair.first == selfName())
            {
                emit playSound(GameSound::KillSound, 1, false);
                ui->slainNameLabel->setText(killPair.second);
                setKillTipsVisible(true);
                killTipsTimer->start();
            }

            if(killMessageToShow.size() == maxKillMessageToShow)
            {
                killMessageToShow.pop_front();
            }
            killMessageToShow.push_back(killPair);
        }
        killMessage.clear();
    }

    for(const auto& t : particleMessage)
    {
        if(t.second == GamePlayingWindow::ParticleSystem::TankBomb)
        {
            QTimer::singleShot(1000, [this](){
                emit playSound(GameSound::TankExplosion, 1, true);
            });

        }
    }
    particleMessage.clear();

    {
        ui->killNumLabel->setNum(selfController.killNum());
        ui->deathNumLabel->setNum(selfController.deathNum());

        //设置武器名称
        QString weaponName;
        int bulletNum = 0;
        if(selfController.tankRefConst().projectileType() == ProjectileActor::MachineGunProjectile)
        {
            bulletNum = selfController.tankRefConst().machineGunBulletNum();
            weaponName = QStringLiteral("重机枪");
        }
        else
        {
            bulletNum = selfController.tankRefConst().armorPiercingBulletNum();
            weaponName = QStringLiteral("穿甲弹");
            if(!selfController.tankRefConst().armorPiercingIsReady() &&
                selfController.tankRefConst().armorPiercingBulletNum() > 0)
                weaponName += QStringLiteral("（装填中...）");
        }
        ui->weaponLabel->setText(weaponName);
        ui->bulletNumLabel->setNum(bulletNum);

        int healthValue = 0;
        if(selfController.isDead())
        {
            healthValue = 0;
        }
        else
        {
            healthValue = selfController.tankRefConst().health();
        }

        ui->healthProgressBar->setValue(healthValue);
        ui->armorProgressBar->setValue(selfController.tankRefConst().armorValue());
    }
}

void GamePlayingWindow::print()
{
    if(!frameBuffer.empty())
    {
        screenShowLabel->setPixmap(QPixmap::fromImage(frameBuffer.front()));
        frameBuffer.pop_front();
    }
}

void GamePlayingWindow::startGame()
{
    computeTimer->start(computeInterval);
    printTimer->start(printInterval);
}

void GamePlayingWindow::handlePacket(QByteArray packet)
{
    QDataStream in(&packet, QIODevice::ReadOnly);
    quint16 type;
    in >> type;

    switch (type) {
    case Config::GameInfoPacket:
    {
        int remainTime, minute, second;
        in >> remainTime;

        remainTime /= 1000;
        minute = remainTime / 60;
        second = remainTime - (minute * 60);

        QString minuteText = QString::number(minute);
        QString secondText = QString::number(second);

        if(minute < 10)
            minuteText = "0" + minuteText;

        if(second < 10)
            secondText = "0" + secondText;

        ui->minuteLcdNumber->display(minuteText);
        ui->secondLcdNumber->display(secondText);

        break;
    }
    case Config::PlayerInfoListPacket:
    {
        dataBuffer[PlayerInfoPacketIndex] = packet;
        break;
    }
    case Config::ProjectileInfoListPacket:
    {
        dataBuffer[ProjectileInfoPacketIndex] = packet;
        break;
    }
    case Config::BlockInfoListPacket:
    {
        int num = 0;
        in >> num;

        BlockActor block;
        for(int i = 0; i < num; ++i)
        {
            in >> block;

            if(block.health() <= 0)
                blockActorHash.remove(block.location());
            else
            {
                block.setSource(blockSource);
                blockActorHash[block.location()] = block;
            }
        }

        break;
    }
    case Config::KillListPacket:
    {
        int num;
        in >> num;

        QString first, second;
        for(int i = 0; i < num; ++i)
        {
           in >> first >> second;
           killMessage.push_back({first, second});
        }

        break;
    }
    case Config::ParticleEmitPacket:
    {
        int num;
        in >> num;

        QPoint location;
        ParticleSystem::ParticleType type;
        for(int i = 0; i < num; ++i)
        {
            in >> location >> type;
            particleMessage.push_back({location, type});
        }

        break;
    }
    case Config::MapInfoPacket:
    {
        in >> _mapSize;
        BlockActor block;
        block.setSource(blockSource);

        block.setSize({_mapSize.width() + wallThick * 2, wallThick});
        block.setLocation({-wallThick, -wallThick});
        boundaryWall[0] = block;    //上面一堵墙

        block.setLocation({-wallThick, _mapSize.height()});
        boundaryWall[1] = block;    //下面一堵墙

        block.setSize({wallThick, _mapSize.height()});
        block.setLocation({-wallThick, 0});
        boundaryWall[2] = block;    //左面一堵墙

        block.setLocation({_mapSize.width(), 0});
        boundaryWall[3] = block;    //右面一堵墙

        break;
    }
    }
}

void GamePlayingWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    PlayerOperation op;
    out << Config::PlayerOperationPacket;

    bool isMove = false;
    switch (event->key()) {
    case Qt::Key_W:
    {
        out << int(2);
        op.setOperation(PlayerOperation::MOVE_FORWARD);
        out << op;

        op.setOperation(PlayerOperation::ROTATE);
        op.setValue(-90);
        out << op;

        moveKeyPress = event->key();
        isMove = true;

        break;
    }
    case Qt::Key_A:
    {
        out << int(2);
        op.setOperation(PlayerOperation::MOVE_LEFT);
        out << op;

        op.setOperation(PlayerOperation::ROTATE);
        op.setValue(180);
        out << op;

        moveKeyPress = event->key();
        isMove = true;

        break;
    }
    case Qt::Key_S:
    {
        out << int(2);
        op.setOperation(PlayerOperation::MOVE_BACKWARD);
        out << op;

        op.setOperation(PlayerOperation::ROTATE);
        op.setValue(90);
        out << op;

        moveKeyPress = event->key();
        isMove = true;

        break;
    }
    case Qt::Key_D:
    {
        out << int(2);
        op.setOperation(PlayerOperation::MOVE_RIGHT);
        out << op;

        op.setOperation(PlayerOperation::ROTATE);
        op.setValue(0);
        out << op;

        moveKeyPress = event->key();
        isMove = true;

        break;
    }
    case Qt::Key_Space:
    {
        out << int(1);
        op.setOperation(PlayerOperation::SPEED_UP);
        out << op;

        break;
    }
    case Qt::Key_F:
    {
        out << int(1);
        op.setOperation(PlayerOperation::SELECT_BULLET);
        out << op;
        emit playSound(GameSound::SelectWeaponSound, 1, false);

        break;
    }
    case Qt::Key_R:
    {
        out << int(1);
        op.setOperation(PlayerOperation::REVIVAL);
        out << op;

        break;
    }
    }

    emit packetToSend(packet);

    if(isMove && !selfController.isDead())
    {
        emit playSound(GameSound::TankMoveSound, QSoundEffect::Infinite, false);
    }
}

void GamePlayingWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return;

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    PlayerOperation op;
    out << Config::PlayerOperationPacket;

    if(event->key() == moveKeyPress)
    {
        out << int(1);
        op.setOperation(PlayerOperation::MOVE_STOP);
        out << op;

        emit stopSound(GameSound::TankMoveSound);
    }
    else if(event->key() == Qt::Key_Space)
    {
        out << int(1);
        op.setOperation(PlayerOperation::SPEED_UP_STOP);
        out << op;
    }
    else;

    emit packetToSend(packet);
}

void GamePlayingWindow::mousePressEvent(QMouseEvent *event)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    PlayerOperation op;
    out << Config::PlayerOperationPacket;

    if(event->button() == Qt::LeftButton)
    {
        out << int(1);
        op.setOperation(PlayerOperation::PRESS_FIRE_BUTTON);
        out << op;
    }

    emit packetToSend(packet);

    if(selfController.isDead())
        return;

    if(selfController.tankRefConst().projectileType() == ProjectileActor::ArmorPiercingProjectile &&
            selfController.tankRefConst().armorPiercingIsReady())
    {
        emit playSound(GameSound::ArmorFireSound, 1, false);
    }

    if(selfController.tankRefConst().projectileType() == ProjectileActor::MachineGunProjectile)
    {
        QTime currentTime = QTime::currentTime();
        int delta = lastClickTime.msecsTo(currentTime);

        if(delta >= 1500)
        {
            lastClickTime = currentTime;
            emit playSound(GameSound::MachineGunFireSound, 1, true);
        }

        machineGunTimer->start();
    }
}

void GamePlayingWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    PlayerOperation op;
    out << Config::PlayerOperationPacket;

    if(event->button() == Qt::LeftButton)
    {
        out << int(1);
        op.setOperation(PlayerOperation::RELEASE_FIRE_BUTTON);
        out << op;
    }

    emit packetToSend(packet);

    machineGunTimer->stop();
    emit stopSound(GameSound::MachineGunFireSound);
}

void GamePlayingWindow::setKillTipsVisible(bool visible)
{
    ui->killLogoLabel->setVisible(visible);
    ui->killTextLabel->setVisible(visible);
    ui->slainNameLabel->setVisible(visible);
}

void GamePlayingWindow::flushKillMessage()
{
    ui->killListWidget->clear();
    for(const auto& killPair : killMessageToShow)
    {
        ui->killListWidget->addItem(killPair.first + "  kill  " + killPair.second);
    }
}


