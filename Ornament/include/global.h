//
// Created by Flache on 2024/12/21.
//

#ifndef ORNAMENT_GLOBAL_H
#define ORNAMENT_GLOBAL_H


#include <QString>
#include <QObject>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
typedef struct  {
    QString userName;
    QString userAccount;
    QString userPassword;
    QString userHead;
}UserInfo;

Q_DECLARE_METATYPE(UserInfo)


class global:public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal windowDpi READ windowDpi NOTIFY WindowDpiChanged)  //系统缩放比例
    Q_PROPERTY(QString userName WRITE setUserName READ userName NOTIFY  userNameChanged) //用户名
    Q_PROPERTY(QString userHead WRITE setUserHead READ userHead NOTIFY userHeadChanged); //用户头像二进制数据
    Q_PROPERTY(QString userAccount WRITE setUserAccount READ userAccount NOTIFY userAccountChanged) //用户账号

public:
    explicit global();
    ~global()override =default;

    void setWindowDpi(const qreal & dpi);
    [[nodiscard]] qreal windowDpi()const;

    void setUserName(const QString & user_name);
    [[nodiscard]] QString userName()const;

    void setUserHead(const QString &imageData);
    [[nodiscard]] QString userHead()const;

    void setUserAccount(const QString  & userAccount);
    [[nodiscard]] QString userAccount()const;

    Q_INVOKABLE static int GetStringWidth(const QString & text);
    Q_INVOKABLE static int GetStringHeight(const QString & text);



private:
    qreal m_windowDpi = 0.0;
    QString m_userName;
    QString  m_userAccount;
    QString m_userHead;

    QMap<QByteArray,QString> friendInfo;
Q_SIGNALS:
    void WindowDpiChanged();
    void LoginSucceed();
    void userNameChanged();
    void userHeadChanged();
    void userAccountChanged();
};

Q_DECLARE_METATYPE(global)

#endif//ORNAMENT_GLOBAL_H
