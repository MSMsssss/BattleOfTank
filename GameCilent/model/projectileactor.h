#ifndef BULLETACTOR_H
#define BULLETACTOR_H
#include "model/actor.h"
#include <QHash>

class PlayerController;

class ProjectileActor : public Actor
{
public:
    using ProjectileType = int;
    static const ProjectileType ArmorPiercingProjectile = 1;    //穿甲弹
    static const ProjectileType MachineGunProjectile = 2;   //机枪弹
    static const int BaseDamageValue = 10;   //基础伤害
    static const int BaseProjectileSpeed = 100; //基础速度

public:
    explicit ProjectileActor(bool isDynamic = true);

    friend QDataStream& operator>>(QDataStream& in, ProjectileActor& actor);
    friend QDataStream& operator<<(QDataStream& out, const ProjectileActor& actor);

    int damageValue() const;
    void setDamageValue(int damageValue);

    PlayerController *controller() const;
    void setController(PlayerController *controller);

    void in(QDataStream &inStream) override;
    void out(QDataStream &outStream) const override;

    ProjectileType type() const;
    void setType(const ProjectileType &type);

protected:
    PlayerController* _controller = nullptr;
    int _damageValue;
    ProjectileType _type;
};

#endif // BULLETACTOR_H
