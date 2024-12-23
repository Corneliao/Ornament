//
// Created by Flache on 2024/12/22.
//

#include "../include/Client.h"
Client::Client(QObject *parent) : QObject(parent) {
    this->thread = new QThread;
    this->connectServer = new ConnectServer();
    this->connectServer->moveToThread(this->thread);
    connect(this->thread,&QThread::started,this->connectServer,&ConnectServer::initializeSocket,Qt::DirectConnection);
    this->thread->start();
}
Client::~Client() {
    this->connectServer->deleteLater();
    this->thread->exit(0);
    this->thread->wait();
    this->thread->deleteLater();

    this->thread = Q_NULLPTR;
    this->connectServer = Q_NULLPTR;
}
