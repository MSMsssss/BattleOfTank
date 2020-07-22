#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;

signals:
    void packetToSend(QByteArray packet);
    void loginSuccess(QString selfName);

public slots:
    void handlePacket(QByteArray packet);

private slots:
    void login();
};

#endif // LOGINDIALOG_H
