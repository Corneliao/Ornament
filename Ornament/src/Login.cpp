//
// Created by Flache on 2024/12/21.
//

#include "../include/Login.h"

Login::Login(const qreal & dpi,QObject *parent) : QObject(parent) {

    qRegisterMetaType<QString>("QString&");
    qRegisterMetaType<QByteArray>("QByteArray&");

    this->view = new  QuickFramelessView(Q_NULLPTR);
    this->view->setSource(QUrl("qrc:/resource/qml/LoginContainer.qml"));
    this->view->show();
    connect(this->view,&QQuickView::statusChanged,this,[=](QQuickView::Status state){
        qDebug() <<state;
    });
    this->rootObject = this->view->rootObject();
    this->setWindowDpi(dpi);
    QQuickItem * title_bar = this->rootObject->childItems().at(0)->childItems().first();
    if(title_bar)
        this->view->setTitleBar(title_bar);

    //初始化数据库线程
    this->thread = new QThread;
    this->userdatabase = new UserDatabaseManager;
    this->userdatabase->moveToThread(this->thread);
    connect(this->thread,&QThread::started,this->userdatabase,&UserDatabaseManager::initializeUserDatabase,Qt::DirectConnection);
    this->thread->start();

    connect(rootObject,SIGNAL(logining(QString,QString)),this->userdatabase,SLOT(CheckTheLoginUserInfo(QString,QString)),Qt::QueuedConnection);
    connect(this->userdatabase,&UserDatabaseManager::UserInfoIsCorrect,this,&Login::UserLogined,Qt::QueuedConnection);
    connect(this->rootObject,SIGNAL(userAccountChanged(QString)),this->userdatabase,SLOT(userAccountChanged(QString)),Qt::QueuedConnection);
    connect(this->userdatabase,&UserDatabaseManager::updateUserHead,this,[=](const QString & imageData){
        this->rootObject->setProperty("imageData",imageData);
        this->rootObject->setProperty("roundImage",true);
        this->rootObject->setProperty("fromImageData",true);
    },Qt::QueuedConnection);
    connect(this->rootObject,SIGNAL(registerAccount(QString,QString,QString)),this->userdatabase,SLOT(RegisterUserAccount(QString,QString,QString)));

}

void Login::destoryQuickView() {
    this->view->close();
    this->rootObject->deleteLater();
    this->view->deleteLater();
    this->rootObject = Q_NULLPTR;
    this->view = Q_NULLPTR;

    this->userdatabase->deleteLater();
    this->thread->exit(0);
    this->thread->wait();
    this->thread->deleteLater();
    this->userdatabase = Q_NULLPTR;
    this->thread = Q_NULLPTR;
}

Login::~Login() {
    if(this->view != Q_NULLPTR) {
        this->destoryQuickView();
    }
}
void Login::setWindowDpi(const qreal &dpi) {
    this->rootObject->setProperty("windowDpi",dpi);
}
