//
// Created by Flache on 2024/12/21.
//

#ifndef ORNAMENT_LOGIN_H
#define ORNAMENT_LOGIN_H
#include "QuickFramelessView.h"

#include <QObject>
class Login: public QObject {
    Q_OBJECT
public:
    explicit Login(QObject * parent = Q_NULLPTR);
    ~Login() override = default ;
    void destoryQuickView();
public slots:
      void test();
private:
    QuickFramelessView * view = Q_NULLPTR;
    QQuickItem * rootObject =Q_NULLPTR;
Q_SIGNALS:
    void  succeed();
};


#endif//ORNAMENT_LOGIN_H
