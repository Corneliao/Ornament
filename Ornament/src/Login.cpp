//
// Created by Flache on 2024/12/21.
//

#include "../include/Login.h"

Login::Login(QObject *parent) : QObject(parent) {
    this->view = new  QuickFramelessView(Q_NULLPTR);
    this->view->setSource(QUrl("qrc:/resource/qml/LoginContainer.qml"));
    this->view->show();
    this->rootObject = this->view->rootObject();
    QQuickItem * title_bar = this->rootObject->childItems().first()->childItems().first();
    if(title_bar)  {
        qDebug() << title_bar->objectName();
        this->view->setTitleBar(title_bar);
    }
    connect(rootObject,SIGNAL(loginSucceed()),this,SLOT(test()),Qt::DirectConnection);
}

void Login::destoryQuickView() {

    this->view->close();
    this->rootObject->deleteLater();
    this->view->deleteLater();
    this->rootObject = Q_NULLPTR;
    this->view = Q_NULLPTR;
}
void Login::test() {
    qDebug()  << "登录成功";
    emit this->succeed();
}
