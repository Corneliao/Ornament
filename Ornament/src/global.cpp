//
// Created by Flache on 2024/12/21.
//

#include "../include/global.h"


global::global() = default;

void global::setWindowDpi(const qreal & dpi) {
    this->m_windowDpi = dpi;
}
qreal global::windowDpi() const {
    return this->m_windowDpi;
}
void global::setUserName(const QString & user_name) {
    this->m_userName =  user_name;
}
QString global::userName() const {
    return this->m_userName;
}
void global::setUserHead(const QString &imageData) {
    this->m_userHead = imageData;
}
QString global::userHead() const {
    return this->m_userHead;
}

void global::setUserAccount(const QString &userAccount) {
    this->m_userAccount = userAccount;
}
QString global::userAccount() const {
    return this->m_userAccount;
}
