//
// Created by Flache on 2024/12/21.
//

#ifndef ORNAMENT_LOGIN_H
#define ORNAMENT_LOGIN_H
#include "../QuickFramelessWindow/QuickFramelessView.h"

#include <QObject>
#include <QThread>
#include "UserDatabaseManager.h"
class Login: public QObject {
    Q_OBJECT
public:

    explicit Login(const qreal & dpi,QObject * parent = Q_NULLPTR);
    ~Login() override;
    void destoryQuickView();

void setWindowDpi(const qreal &dpi);
private:
    QuickFramelessView * view = Q_NULLPTR;
    QQuickItem * rootObject =Q_NULLPTR;
signals:
    void UserLogined(const QString &  userName,const QString & account,const QString & imageData);
private:
    QThread * thread = Q_NULLPTR;
    UserDatabaseManager * userdatabase = Q_NULLPTR;
};


#endif//ORNAMENT_LOGIN_H
