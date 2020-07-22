QT       += core gui \
            network \
            sql \
            multimedia \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    function.cpp \
    gamecilent.cpp \
    gamemap.cpp \
    gamemode.cpp \
    gameplayingwindow.cpp \
    gameroom.cpp \
    gamesound.cpp \
    helpfultool.cpp \
    logindialog.cpp \
    main.cpp \
    gamemenuwindow.cpp \
    model/actor.cpp \
    model/blockactor.cpp \
    model/gamecamera.cpp \
    model/playercontroller.cpp \
    model/projectileactor.cpp \
    model/squarecomponent.cpp \
    model/tankactor.cpp \
    packettcpsocket.cpp \
    playeroperation.cpp

HEADERS += \
    config.h \
    function.h \
    gamecilent.h \
    gamemap.h \
    gamemenuwindow.h \
    gamemode.h \
    gameplayingwindow.h \
    gameroom.h \
    gamesound.h \
    helpfultool.h \
    logindialog.h \
    model/actor.h \
    model/blockactor.h \
    model/gamecamera.h \
    model/playercontroller.h \
    model/projectileactor.h \
    model/squarecomponent.h \
    model/tankactor.h \
    packettcpsocket.h \
    playeroperation.h

FORMS += \
    gamemenuwindow.ui \
    gameplayingwindow.ui \
    logindialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
