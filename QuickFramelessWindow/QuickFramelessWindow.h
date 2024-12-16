//
// Created by Flache on 2024/12/16.
//

#ifndef UNTITLED_QUICKFRAMELESSWINDOW_H
#define UNTITLED_QUICKFRAMELESSWINDOW_H

#include <QQuickWindow>
#include <QObject>
#include <windowsx.h>
#include <windows.h>
#include <dwmapi.h>
#include <QEvent>
#include <QQuickItem>

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

protected:

private:
    void setWindowsStyle();

    int adjustResizeWindow(HWND hwnd, const QPoint &mouse_pos);

    void adjustResizeContentMargins(bool isMaximized);

    int dealWindowTitleBar(const int &x, const int &y);

    QQuickItem *m_titleBar = Q_NULLPTR;
    QList<QQuickItem *> whitelistItem;
};


#endif //UNTITLED_QUICKFRAMELESSWINDOW_H
