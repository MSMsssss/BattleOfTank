#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QString>
#include <QHash>

class GameMap
{
public:
    using MapType = int;

public:
    GameMap();
    static QString getDescription(MapType gameMap);
    static QString getName(MapType gameMap);

public:
    static const MapType MapOne = 1;
    static const MapType MapTwo = 2;
    static const MapType MapThree = 3;

private:
    static const QHash<MapType, QString> descriptionHash;
    static const QHash<MapType, QString> nameHash;
};

#endif // GAMEMAP_H
