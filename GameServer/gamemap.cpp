#include "gamemap.h"

GameMap::GameMap()
{

}

const QHash<GameMap::MapType, QString> GameMap::descriptionHash = {
    {GameMap::MapOne, QStringLiteral("一号地图")},
    {GameMap::MapTwo, QStringLiteral("二号地图")},
    {GameMap::MapThree, QStringLiteral("三号地图")}
};

const QHash<GameMap::MapType, QString> GameMap::nameHash = {
    {GameMap::MapOne, QStringLiteral("一号地图")},
    {GameMap::MapTwo, QStringLiteral("二号地图")},
    {GameMap::MapThree, QStringLiteral("三号地图")}
};

QString GameMap::getDescription(GameMap::MapType gameMap)
{
    if(descriptionHash.contains(gameMap))
        return descriptionHash[gameMap];
    else
        return "";
}

QString GameMap::getName(GameMap::MapType gameMap)
{
    if(nameHash.contains(gameMap))
        return nameHash[gameMap];
    else
        return "";
}
