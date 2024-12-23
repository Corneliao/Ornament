//
// Created by Flache on 2024/12/22.
//

#include "../include/ConnectServer.h"
ConnectServer::ConnectServer(QObject *parent) : QObject(parent) {
}
ConnectServer::~ConnectServer() {
}
void ConnectServer::initializeSocket() {
    this->socket = new QTcpSocket(this);
    this->socket->setSocketOption(QAbstractSocket::KeepAliveOption,true);
    this->socket->connectToHost(QHostAddress("127.0.0.1"),quint16(7502));
    if(!this->socket->waitForConnected()) {
        qDebug() << "连接失败";
        return;
    }
    connect(this->socket,&QTcpSocket::readyRead,this,&ConnectServer::ReceiveSocketData,Qt::DirectConnection);
}
void ConnectServer::ReceiveSocketData() {
}
void ConnectServer::sendUserTextMessage(const QString &senderUser, const QString &recverUser, const QString &messagetext) {
    QByteArray out;
    QDataStream stream(&out,QIODevice::WriteOnly);
    int type = MSGTYPE::TEXT;
    stream << type<< senderUser << recverUser << messagetext;
    this->socket->write(out);
    if(!this->socket->waitForBytesWritten())  {
        qDebug() << "发送失败" << this->socket->errorString();
    }
}
