#ifndef SERVERCONTROLLERWINDOW_H
#define SERVERCONTROLLERWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerControllerWindow; }
QT_END_NAMESPACE

class ServerControllerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerControllerWindow(QWidget *parent = nullptr);
    ~ServerControllerWindow();

private:
    Ui::ServerControllerWindow *ui;
};
#endif // SERVERCONTROLLERWINDOW_H
