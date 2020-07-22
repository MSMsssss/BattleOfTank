#ifndef TANKACTOR_H
#define TANKACTOR_H

#include "model/actor.h"
#include "model/projectileactor.h"
class QTimer;
class PlayerController;

class TankActor : public Actor
{
public:
    explicit TankActor(bool isDynamic = true);

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

    int machineGunBulletNum() const;

    int armorPiercingBulletNum() const;

    bool armorPiercingIsReady() const;

protected:
    PlayerController* _controller;

    int _health = 100;  //生命值
    int _armorValue = 0;    //护甲值

    int _speedUpRate = 1;
    int _bulletSpeedAddition = 1;   // 子弹速度增益
    int _bulletDamageAddition = 1;  //  子弹伤害增益

    ProjectileActor::ProjectileType _projectileType = ProjectileActor::MachineGunProjectile;    //当前装备的弹药
    int _machineGunBulletNum = 2000;
    int _armorPiercingBulletNum = 25;

    bool _armorPiercingIsReady = true;
    bool _machineGunIsReady = true;
    bool _isAutoFire = false;
};

#endif // TANKACTOR_H
