#include "widget.h"
#include "./ui_widget.h"
#include "messengerconnectiondialog.h"
#include <QTcpSocket>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    mSocket = new QTcpSocket(this);

    connect(mSocket, &QTcpSocket::readyRead, [&]()
    {
        QTextStream T(mSocket);
        auto text = T.readAll();
        ui->textEdit->append(text);
    });
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_send_clicked()
{
    QTextStream T(mSocket);
    T << ui->nickname->text() << ": " << ui->message->text();
    mSocket->flush();
    ui->message->clear();

}

void Widget::on_connect_clicked()
{
    MessengerConnectionDialog D(this);
    if (D.exec() == QDialog::Rejected)
    {
        return;
    }
    mSocket->connectToHost(D.hostname(), D.port());


}



