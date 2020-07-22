#ifndef CONFIG_H
#define CONFIG_H
#include <QHostAddress>
#include <QHash>

class Config
{
public:
    Config();

public:
    static QHostAddress serverAddress;
    static quint16 serverPort;

    // 登录状态回复
    static const int LoginSuccess = 1;
    static const int PlayerNameError = 2;
    static const int PasswordError = 3;
    static const int RepeatLogin = 4;

    // 请求加入游戏回复
    static const int RoomIsFull = 1;
    static const int JoinSuccess = 2;
    static const int RoomNotExists = 3;
    static const int RepeatJoin = 4;

    /********************************************
     * 每个数据包长度不定，前两个字节为数据长度，后两个字节为
     * 数据类型。
     * ******************************************/
    static const quint16 EmptyPacket = 0;   //固定尺寸的无效数据包，用于督促操作系统尽快发送数据
    static const quint16 LoginRequestPacket = 1; //登陆请求，包含昵称和密码。
    static const quint16 LoginReplyPacket = 2; //登陆回复，返回登陆结果。
    static const quint16 RegisterRequestPacket = 3;  //注册请求
    static const quint16 RegisterReplyPacket = 4;  //注册结果回复
    static const quint16 PlayerOperationPacket = 5; //玩家操作
    static const quint16 RoomsInfoRequestPacket = 6; //房间信息请求数据包
    static const quint16 ReplyRoomsInfoRequestPacket = 7; //回复房间信息请求数据包
    static const quint16 GameStatusPacket = 8;  //  游戏状态数据包
    static const quint16 RequestJoinRoomPacket = 9; //请求加入房间数据包
    static const quint16 ReplyJoinRoomPacket = 10;   //回复加入房间数据包
    static const quint16 PlayerInfoListPacket = 11;  //玩家信息包
    static const quint16 ProjectileInfoListPacket = 12; // 子弹信息包
    static const quint16 BlockInfoListPacket = 13; //墙体信息包
    static const quint16 KillListPacket = 14;   //击杀列表包
    static const quint16 ParticleEmitPacket = 15;   //粒子发射包
    static const quint16 MapInfoPacket = 16;    //地图信息包
    static const quint16 GameInfoPacket = 17;    //游戏局势信息

    static const bool TestMode = true;

    // 逻辑帧间隔时间，单位ms
    static const int FrameInterval = 50;

    // 仅作测试使用
    static const QHash<QString, QString> playerAccountList;
};

#endif // CONFIG_H
