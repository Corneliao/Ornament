//
// Created by Flache on 2024/12/21.
//

#include "../include/global.h"


global::global() {

}
void global::setWindowDpi(const qreal & dpi) {
    this->m_windowDpi = dpi;
}
qreal global::getWindowDpi() const {
    return this->m_windowDpi;
}
void global::succeed() {
    emit this->LoginSucceed();
}
