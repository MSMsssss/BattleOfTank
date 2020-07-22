#ifndef PLAYEROPERATION_H
#define PLAYEROPERATION_H

#include<QDataStream>

class PlayerOperation
{
public:
    using OperationType = int;
    PlayerOperation();
    PlayerOperation(OperationType op, int val);

    OperationType operation() const;
    void setOperation(const OperationType &operation);

    int value() const;
    void setValue(int value);

public:
    /*  移动操作    */
    static const OperationType MOVE_FORWARD = 1;
    static const OperationType MOVE_BACKWARD = 2;
    static const OperationType MOVE_LEFT = 3;
    static const OperationType MOVE_RIGHT = 4;
    static const OperationType MOVE_STOP = 5;
    static const OperationType SPEED_UP = 6;
    static const OperationType SPEED_UP_STOP = 7;
    static const OperationType ROTATE = 8;

    /*  战斗操作    */
    static const OperationType PRESS_FIRE_BUTTON = 9;
    static const OperationType RELEASE_FIRE_BUTTON = 10;
    static const OperationType SELECT_BULLET = 11;
    static const OperationType REVIVAL = 12;

    friend QDataStream& operator>>(QDataStream& in, PlayerOperation& playerOp);
    friend QDataStream& operator<<(QDataStream& out, const PlayerOperation& playerOp);

protected:
    OperationType _operation;
    int _value;
};

#endif // PLAYEROPERATION_H
