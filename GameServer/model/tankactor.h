#ifndef TANKACTOR_H
#define TANKACTOR_H

#include <QObject>
#include "model/actor.h"
#include "model/projectileactor.h"
class QTimer;
class PlayerController;

class TankActor : public Actor
{
    Q_OBJECT

public:
    static const QSize InitialSize;
    explicit TankActor(QObject *parent = nullptr, GameRunning* gameRunning = nullptr, bool isDynamic = true);

    friend QDataStream& operator>>(QDataStream& in, TankActor& actor);
    friend QDataStream& operator<<(QDataStream& out, const TankActor& actor);

    int health() const;
    void setHealth(int health);

    int armorValue() const;
    void setArmorValue(int armorValue);

    ProjectileActor::ProjectileType projectileType() const;
    void setProjectileType(const ProjectileActor::ProjectileType &projectileType);
    void switchProjectile();

    int bulletSpeedAddition() const;
    void setBulletSpeedAddition(int bulletSpeedAddition);

    int bulletDamageAddition() const;
    void setBulletDamageAddition(int bulletDamageAddition);

    PlayerController *controller() const;
    void setController(PlayerController *controller);

    void in(QDataStream &inStream) override;
    void out(QDataStream &outStream) const override;

    int speedUpRate() const;
    void setSpeedUpRate(int speedUpRate);

    Vector2D velocity() const;

    bool isAutoFire() const;
    void setIsAutoFire(bool isAutoFire);

    int machineGunBulletNum() const;
    int armorPiercingBulletNum() const;

protected:
    PlayerController* _controller;

    int _health = 100;  //生命值
    int _armorValue = 100;    //护甲值

    int _speedUpRate = 1;
    int _bulletSpeedAddition = 1;   // 子弹速度增益
    int _bulletDamageAddition = 1;  //  子弹伤害增益

    ProjectileActor::ProjectileType _projectileType = ProjectileActor::MachineGunProjectile;    //当前装备的弹药
    int _machineGunBulletNum = 2000;
    int _armorPiercingBulletNum = 25;

    QTimer* _machineGunTimer = nullptr;
    QTimer* _armorPiercingTimer = nullptr;

    int _armorPiercingReloadTime = 5000;    //ms
    int _machineGunReloadTime = 500;    //ms

    bool _armorPiercingIsReady = true;
    bool _machineGunIsReady = true;
    bool _isAutoFire = false;

signals:

public slots:
    void tick() override;
    void onHit(Actor *otherActor) override;
    void fire();
    void getDamage(PlayerController* attacker, int damageValue);
};

#endif // TANKACTOR_H
