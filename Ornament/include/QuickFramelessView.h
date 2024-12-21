//
// Created by Flache on 2024/12/21.
//

#ifndef ORNAMENT_QUICKFRAMELESSVIEW_H
#define ORNAMENT_QUICKFRAMELESSVIEW_H

#define  RESIZE_EDGE_THREHDLE 8
#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickView>
#include <Windows.h>
#include <dwmapi.h>
#include <windowsx.h>

class QuickFramelessView: public QQuickView {

    Q_OBJECT
    QML_ELEMENT
public:
    explicit QuickFramelessView(QWindow *parent = Q_NULLPTR);

    ~QuickFramelessView() override = default;

    Q_INVOKABLE void setTitleBar(QQuickItem *title_bar);

    Q_INVOKABLE void addTitleWhitelist(QQuickItem *item);


protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;

    bool event(QEvent *event) override;


private:
    void setWindowsStyle();

    int adjustResizeWindow(HWND hwnd, const QPoint &mouse_pos);

    void adjustResizeContentMargins(bool isMaximized);

    int dealWindowTitleBar(const int &x, const int &y);

    QQuickItem *m_titleBar = Q_NULLPTR;
    QList<QQuickItem *> whitelistItem;
    qreal m_dpi = 0.0;
    int screen_widht = 0;
    int screen_height =0;
};


#endif//ORNAMENT_QUICKFRAMELESSVIEW_H
