//
// Created by Flache on 2024/12/22.
//

#ifndef ORNAMENT_CLIENT_H
#define ORNAMENT_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include "ConnectServer.h"
class Client:public QObject {
    Q_OBJECT
public:
    explicit Client(QObject * parent = Q_NULLPTR);
    ~Client()override;

public:
  //  Q_INVOKABLE void sendUserTextMessage();

private:
    QTcpSocket * socket = Q_NULLPTR;
    QThread * thread =  Q_NULLPTR;
    ConnectServer * connectServer = Q_NULLPTR;
};


#endif//ORNAMENT_CLIENT_H
