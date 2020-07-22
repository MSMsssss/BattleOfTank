#include "servercontrollerwindow.h"
#include "ui_servercontrollerwindow.h"

ServerControllerWindow::ServerControllerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerControllerWindow)
{
    ui->setupUi(this);
}

ServerControllerWindow::~ServerControllerWindow()
{
    delete ui;
}

