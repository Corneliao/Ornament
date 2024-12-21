//
// Created by Flache on 2024/12/21.
//

#ifndef ORNAMENT_GLOBAL_H
#define ORNAMENT_GLOBAL_H


#include <QObject>
#include <QDebug>
class global:public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal WindowDpi READ getWindowDpi  NOTIFY WindowDpiChanged)

public:
    explicit global();
    ~global() =default;
    void setWindowDpi(const qreal & dpi);
    qreal getWindowDpi()const;

    Q_INVOKABLE void succeed();
private:
    qreal m_windowDpi = 0.0;
    QString m_userName;
Q_SIGNALS:
    void WindowDpiChanged();
    void LoginSucceed();
};


#endif//ORNAMENT_GLOBAL_H
