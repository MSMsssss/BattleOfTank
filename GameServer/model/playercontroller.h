#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
class TankActor;
class PlayerOperation;
class GameRunning;

class PlayerController : public QObject
{
    Q_OBJECT
public:
    static const int BLUE_COMPANY = 1;
    static const int RED_COMPANY = 2;

public:
    friend QDataStream& operator>>(QDataStream& inStream, PlayerController& controller);
    friend QDataStream& operator<<(QDataStream& outStream, const PlayerController& controller);

    explicit PlayerController(QObject *parent = nullptr);
    void applyOperation(const PlayerOperation& op);

    QString playerName() const;
    void setPlayerName(const QString &playerName);

    GameRunning *gameRunning() const;
    void setGameRunning(GameRunning *gameRunning);

    TankActor *tank() const;
    void setTank(TankActor *tank);

    int killNum() const;
    void setKillNum(int killNum);

    int deathNum() const;
    void setDeathNum(int deathNum);

    int scores() const;
    void setScores(int scores);

    int company() const;
    void setCompany(int company);

    int initialTankSpeed() const;
    void setInitialTankSpeed(int initialTankSpeed);

    int speedUpRate() const;
    void setSpeedUpRate(int speedUpRate);

    bool isDead() const;
protected:
    QString _playerName = "";
    GameRunning* _gameRunning = nullptr;
    TankActor* _tank = nullptr;
    int _killNum = 0;
    int _deathNum = 0;
    int _scores = 0;
    int _company = 0;
    int _initialTankSpeed = 8;  // 每逻辑帧移动的像素点数
    int _speedUpRate = 2;    //  加速度
    bool _isDead = false;

public slots:
    void revive();
    void die();
};

#endif // PLAYERCONTROLLER_H
