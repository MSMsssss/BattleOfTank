#include "playeroperation.h"

PlayerOperation::PlayerOperation()
{

}

PlayerOperation::PlayerOperation(PlayerOperation::OperationType op, int val) :
    _operation(op), _value(val)
{

}

PlayerOperation::OperationType PlayerOperation::operation() const
{
    return _operation;
}

void PlayerOperation::setOperation(const OperationType &operation)
{
    _operation = operation;
}

int PlayerOperation::value() const
{
    return _value;
}

void PlayerOperation::setValue(int value)
{
    _value = value;
}

QDataStream& operator>>(QDataStream& in, PlayerOperation& playerOp)
{
    in >> playerOp._operation;
    if(playerOp._operation == PlayerOperation::ROTATE)
    {
        in >> playerOp._value;
    }

    return in;
}

QDataStream& operator<<(QDataStream& out, const PlayerOperation& playerOp)
{
    out << playerOp._operation;
    if(playerOp._operation == PlayerOperation::ROTATE)
    {
        out << playerOp._value;
    }

    return out;
}
