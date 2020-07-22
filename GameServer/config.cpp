#include "config.h"

Config::Config()
{

}

QHostAddress Config::serverAddress = QHostAddress("192.168.192.194");
//QHostAddress Config::serverAddress = QHostAddress("127.0.0.1");

quint16 Config::serverPort = 8080;

const QHash<QString, QString> Config::playerAccountList = {{"msm666", "msm666"},
                                                           {"msm111", "msm111"},
                                                           {"msm222", "msm222"},
                                                           {"msm333", "msm333"}};
