#ifndef BLOCKACTOR_H
#define BLOCKACTOR_H

#include <QObject>
#include "model/actor.h"

class BlockActor : public Actor
{
    Q_OBJECT
public:
    explicit BlockActor(QObject *parent = nullptr, GameRunning* gameRunning = nullptr, bool isDynamic = true);

    friend QDataStream& operator>>(QDataStream& in, BlockActor& actor);
    friend QDataStream& operator<<(QDataStream& out, const BlockActor& actor);

    int health() const;
    void setHealth(int health);

    void in(QDataStream &inStream) override;
    void out(QDataStream &outStream) const override;

public:
    static const int InitialHealth = 500;
    static const QSize InitialSize;
    // 每个逻辑帧后，墙体的状态是否改变
    bool _isStatusChanged = false;

protected:
    int _health = InitialHealth;

signals:

public slots:
    void onHit(Actor *otherActor) override;
    void tick() override;
    void getDamage(int damageValue);
};

#endif // BLOCKACTOR_H
