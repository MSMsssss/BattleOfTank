#include "servercontrollerwindow.h"
#include "gameserver.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameServer server;
    server.app = &a;

    return server.run();
}
