#include "gamemode.h"

const QHash<GameMode::ModeType, QString> GameMode::descriptionHash = {
    {GameMode::ConquestMode,
     QStringLiteral("征服模式：\n"
     "游戏双方需要对地图中的几个关键点进行占领\n"
     "每占领一个关键点，本方得分速度+20%，首先\n"
     "达到目标分数的一方获胜。")},

    {GameMode::BreakThroughMode,
     QStringLiteral("突破模式：\n"
     "游戏双方各自有三处需要保卫的目标，率先摧毁"
     "对方三处目标的一方获胜。")},

    {GameMode::TeamDeathmatchMode,
     QStringLiteral("团队死斗："
     "游戏中预先达到目标杀敌数的一方获胜")}
};

const QHash<GameMode::ModeType, QString> GameMode::nameHash = {
    {GameMode::ConquestMode, QStringLiteral("征服模式")},
    {GameMode::BreakThroughMode, QStringLiteral("突破模式")},
    {GameMode::TeamDeathmatchMode, QStringLiteral("团队死斗")}
};

GameMode::GameMode()
{

}

QString GameMode::getDescription(GameMode::ModeType gameMode)
{
    if(descriptionHash.contains(gameMode))
        return descriptionHash[gameMode];
    else
        return "";
}

QString GameMode::getName(GameMode::ModeType gameMode)
{
    if(nameHash.contains(gameMode))
        return nameHash[gameMode];
    else
        return "";
}
