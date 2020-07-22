#include "gamecilent.h"

#include <QApplication>
#include "gameplayingwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameCilent cilent;
    cilent.app = &a;

    return cilent.run();

//    auto w = new GamePlayingWindow();
//    w->show();

//    return a.exec();
}
