//
// Created by Flache on 2024/12/22.
//

#include "../include/UserDatabaseManager.h"
UserDatabaseManager::UserDatabaseManager(QObject *parent) : QObject(parent) {
    qRegisterMetaType<QString>("QString&");
    qRegisterMetaType<QByteArray>("QByteArray&");
}
UserDatabaseManager::~UserDatabaseManager() {
    {
        QSqlDatabase db = QSqlDatabase::database(this->connectionName);
        db.close();
    }
    QSqlDatabase::removeDatabase(this->connectionName);;
    qDebug()  <<  "关闭数据库";
}

/**
 * @brief 初始化数据库
 */
void UserDatabaseManager::initializeUserDatabase() {

    qDebug() << __FUNCTION__  << __LINE__ << "数据库线程" << QThread::currentThreadId();
    this->connectionName = QString::number(quintptr(QThread::currentThread()));
    QSqlDatabase  database = QSqlDatabase::addDatabase("QMYSQL",this->connectionName);
    database.setHostName("localhost");
    database.setPort(3306);
    database.setDatabaseName("user");
    database.setPassword("y..750242");
    database.setUserName("root");
    if(!database.open()) {
        qDebug() <<database.lastError();
    }
    this->isConnectedDatabase = true;
}

/**
 * @brief 匹对用户信息
 * @param userAccount
 * @param userPassword
 */
void UserDatabaseManager::CheckTheLoginUserInfo(const QString &userAccount, const QString &userPassword) {
    UserInfo info = this->isExistTheUser(userAccount);
    if(info.userName.isEmpty())   {
        qDebug() <<  "not  exist";
        return;
    }
    this->userInfo_temp.userName = info.userName;
    this->userInfo_temp.userAccount = info.userAccount;
    if(info.userPassword  ==  userPassword)
        emit this->UserInfoIsCorrect(this->userInfo_temp.userName,this->userInfo_temp.userAccount,this->userInfo_temp.userHead);
    else {
        qDebug()  << "账号或密码错误";
    }
}

/**
 * @brief 是否存在此用户
 * @param userAccount
 * @return
 */
UserInfo UserDatabaseManager::isExistTheUser(const QString &userAccount) {
    QSqlDatabase db = QSqlDatabase::database(this->connectionName);
    QSqlQuery  query(db);
    query.prepare("SELECT userAccount,userPassword,userName FROM usersinfo");
    if(query.exec()) {
        while(query.next()) {
            QString account = query.value(0).toString();
            if(account == userAccount){
               QString password = query.value(1).toString();
               QString userName = query.value(2).toString();
               UserInfo info =  {userName,account,password};
               return info;
            }
        }
    }
    return {};
}

/**
 * @brief 账号发生变化时用户用户头像
 * @param userAccount
 */
void UserDatabaseManager::userAccountChanged(const QString &userAccount) {
    //获取用户头像
    QSqlDatabase db = QSqlDatabase::database(this->connectionName);
    QSqlQuery query(db);
    query.prepare("SELECT userAccount,userHead FROM usersinfo");
    if(query.exec())  {
        while(query.next()) {
            QString account = query.value(0).toString();
            if(account  == userAccount) {
                QString imageData = query.value(1).toString();
                this->userInfo_temp.userHead = imageData;
                emit this->updateUserHead(imageData);
                return;
            }
        }
    }
}
