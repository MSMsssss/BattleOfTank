#include "logindialog.h"
#include "ui_logindialog.h"
#include "config.h"
#include "function.h"
#include <QPixmap>
#include <QLabel>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    QPixmap logo("./picture/Tank_2.png");
    logo = logo.scaled(logo.width() / 4, logo.height() / 4);
    ui->logoLabel->setFixedSize(logo.width(), logo.height());
    ui->logoLabel->setPixmap(logo);

    connect(ui->loginPushButton, &QPushButton::clicked, this, &LoginDialog::login);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::handlePacket(QByteArray packet)
{
    quint16 type;
    QDataStream in(&packet, QIODevice::ReadOnly);
    in >> type;

    if(type == Config::LoginReplyPacket)
    {
        int status;
        in >> status;

        Function::initLabel(ui->messageLabel);
        if(status == Config::LoginSuccess)
        {
            ui->messageLabel->setText(QStringLiteral("登录成功"));
            emit loginSuccess(ui->playerNameLineEdit->text());
        }
        else if (status == Config::PlayerNameError) {
            ui->messageLabel->setText(QStringLiteral("用户名错误"));
            ui->loginPushButton->setDisabled(false);
            ui->playerNameLineEdit->setDisabled(false);
            ui->passwordLineEdit->setDisabled(false);
        }
        else if (status == Config::PasswordError) {
            ui->messageLabel->setText(QStringLiteral("密码错误"));
            ui->loginPushButton->setDisabled(false);
            ui->playerNameLineEdit->setDisabled(false);
            ui->passwordLineEdit->setDisabled(false);
        }
        else if (status == Config::RepeatLogin) {
            ui->messageLabel->setText(QStringLiteral("该账户已登录"));
            ui->loginPushButton->setDisabled(false);
            ui->playerNameLineEdit->setDisabled(false);
            ui->passwordLineEdit->setDisabled(false);
        }
        else{
            ui->messageLabel->setText(QStringLiteral("登录失败"));
            ui->loginPushButton->setDisabled(false);
            ui->playerNameLineEdit->setDisabled(false);
            ui->passwordLineEdit->setDisabled(false);
        }
    }
}

void LoginDialog::login()
{
    ui->loginPushButton->setDisabled(true);
    ui->playerNameLineEdit->setDisabled(true);
    ui->passwordLineEdit->setDisabled(true);

    QString playerName = ui->playerNameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    QByteArray packet;
    QDataStream(&packet, QIODevice::WriteOnly) << Config::LoginRequestPacket
                                               << playerName
                                               << password;
    emit packetToSend(packet);
}
