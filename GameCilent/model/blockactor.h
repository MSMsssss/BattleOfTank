#ifndef BLOCKACTOR_H
#define BLOCKACTOR_H

#include "model/actor.h"

class BlockActor : public Actor
{
public:
    static const int InitialHealth = 500;
    explicit BlockActor(bool isDynamic = true);

    friend QDataStream& operator>>(QDataStream& in, BlockActor& actor);
    friend QDataStream& operator<<(QDataStream& out, const BlockActor& actor);

    int health() const;
    void setHealth(int health);

    void in(QDataStream &inStream) override;
    void out(QDataStream &outStream) const override;

    QImage printSource() const override;

public:
    // 每个逻辑帧后，墙体的状态是否改变
    bool _isStatusChanged = false;

protected:
    int _health = InitialHealth;
};

#endif // BLOCKACTOR_H
