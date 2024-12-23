//
// Created by Flache on 2024/12/22.
//

#ifndef ORNAMENT_USERDATABASEMANAGER_H
#define ORNAMENT_USERDATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>
#include <QSqlError>
#include  "global.h"
class UserDatabaseManager:public QObject {
    Q_OBJECT
public:
    explicit UserDatabaseManager(QObject  * parent = Q_NULLPTR);
    ~UserDatabaseManager()override;

    void initializeUserDatabase();
    /**
     * @brief 判断用户是否存在
     * @param userAccount
     */
    UserInfo isExistTheUser(const QString & userAccount);
public slots:
    /**
     * @brief 匹配用户数据
     * @param userAccount
     * @param userPassword
     */
    void CheckTheLoginUserInfo(const QString & userAccount,const QString & userPassword);
    /**
     * @brief 当用户账号变化获取用户头像
     * @param userAccount
     */
    void userAccountChanged(const QString & userAccount);
private:
    QString connectionName;
    bool isConnectedDatabase  = false;
    UserInfo userInfo_temp;
signals:
    /**
     * @brief 用户数据匹对成功发送此信号
     * @param userName
     * @param userAccount
     * @param imageData
     */
    void UserInfoIsCorrect(const QString & userName,const QString & userAccount,const QString &imageData);
    /**
     * @brief 返回用户头像数据
     * @param data
     */
    void userHeadByteArray(const QString & data);
    /**
     * @brief 更新显示用户头像
     * @param imageData
     */
    void updateUserHead(const QString &  imageData);
};


#endif//ORNAMENT_USERDATABASEMANAGER_H
