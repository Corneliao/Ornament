//
// Created by Flache on 2024/12/16.
//

#ifndef UNTITLED_QUICKFRAMELESSWINDOW_H
#define UNTITLED_QUICKFRAMELESSWINDOW_H

#include <QEvent>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>
#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>

#define RESIZE_EDGE_THREHDLE 8

class QuickFramelessWindow : public QQuickWindow {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit QuickFramelessWindow(QWindow *parent = Q_NULLPTR);

    ~QuickFramelessWindow() override = default;

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
};


#endif//UNTITLED_QUICKFRAMELESSWINDOW_H
