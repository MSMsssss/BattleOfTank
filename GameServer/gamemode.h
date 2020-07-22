#ifndef GAMEMODE_H
#define GAMEMODE_H
#include <QHash>
#include <QString>

class GameMode
{

public:
    using ModeType = int;

public:
    GameMode();
    static QString getDescription(ModeType gameMode);
    static QString getName(ModeType gameMode);

public:
    static const ModeType ConquestMode = 1;
    static const ModeType TeamDeathmatchMode = 2;
    static const ModeType BreakThroughMode = 3;

private:
    static const QHash<ModeType, QString> descriptionHash;
    static const QHash<ModeType, QString> nameHash;
};

#endif // GAMEMODE_H
