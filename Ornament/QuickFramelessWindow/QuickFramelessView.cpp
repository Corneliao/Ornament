//
// Created by Flache on 2024/12/21.
//

#include "QuickFramelessView.h"
#include <QGuiApplication>
#include <QScreen>
QuickFramelessView::QuickFramelessView(QWindow *parent) : QQuickView(parent) {
    //设置窗口为无边框
    this->m_dpi = this->devicePixelRatio();
    QScreen *screen = QGuiApplication::primaryScreen();
    this->setFlags(flags() | Qt::WindowType::Window);

    //设置窗口风格
    this->setMinimumSize(QSize(320,400));
    this->setWindowsStyle();
    this->setColor(QColor::fromString("#eff1f2"));
    //窗口居中
    this->setX((screen->size().width()-this->width())/2);
    this->setY((screen->size().height()-this->height())/2);

}

void QuickFramelessView::setWindowsStyle() {
    auto hwnd = reinterpret_cast<HWND>(this->winId());
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

    const MARGINS shadow = {1, 1, 1, 1};
    ::DwmExtendFrameIntoClientArea(hwnd, &shadow);
}

bool QuickFramelessView::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
    MSG *msg = reinterpret_cast<MSG *>(message);
    switch (msg->message) {
        case WM_NCCALCSIZE: {
//            *result = 0;
//            return true;
            *result = WVR_REDRAW;
            return true;
        }
        case WM_NCHITTEST: {
            int x = GET_X_LPARAM(msg->lParam);
            int y = GET_Y_LPARAM(msg->lParam);
            QPoint mouse_pos(x, y);
            *result = this->adjustResizeWindow(msg->hwnd, mouse_pos);
            if (*result != 0)
                return true;
            *result = this->dealWindowTitleBar(x, y);
            if (*result != 0)
                return true;
            return false;
        }
        default:
            return QWindow::nativeEvent(eventType, message, result);
    }
}

int QuickFramelessView::adjustResizeWindow(HWND hwnd, const QPoint &mouse_pos) {
    int result = 0;

    RECT window_rect = {0, 0, 0, 0};
    ::GetWindowRect(hwnd, &window_rect);


    bool allow_resize_width = this->minimumWidth() != this->maximumWidth();
    bool allow_resize_height = this->minimumHeight() != this->maximumHeight();
    int mouse_x = mouse_pos.x();
    int mouse_y = mouse_pos.y();

    if (allow_resize_width) {
        if (mouse_x >= window_rect.left && mouse_x < window_rect.left + RESIZE_EDGE_THREHDLE) {
            result = HTLEFT;
        } else if (mouse_x <= window_rect.right && mouse_x > window_rect.right - RESIZE_EDGE_THREHDLE) {
            result = HTRIGHT;
        }
    }
    if (allow_resize_height) {
        if (mouse_y >= window_rect.top && mouse_y < window_rect.top + RESIZE_EDGE_THREHDLE) {
            result = HTTOP;
        } else if (mouse_y <= window_rect.bottom && mouse_y > window_rect.bottom - RESIZE_EDGE_THREHDLE) {
            result = HTBOTTOM;
        }
    }

    if (allow_resize_width && allow_resize_height) {
        if (mouse_x >= window_rect.left && mouse_x < window_rect.left + RESIZE_EDGE_THREHDLE &&
            mouse_y >= window_rect.top && mouse_y < window_rect.top + RESIZE_EDGE_THREHDLE) {
            result = HTTOPLEFT;
        } else if (mouse_y >= window_rect.top && mouse_y < window_rect.top + RESIZE_EDGE_THREHDLE &&
                   mouse_x <= window_rect.right && mouse_x > window_rect.right - RESIZE_EDGE_THREHDLE) {
            result = HTTOPRIGHT;
        } else if (mouse_x >= window_rect.left && mouse_x < window_rect.left + RESIZE_EDGE_THREHDLE &&
                   mouse_y <= window_rect.bottom && mouse_y > window_rect.bottom - RESIZE_EDGE_THREHDLE) {
            result = HTBOTTOMLEFT;
        } else if (mouse_x <= window_rect.right && mouse_x > window_rect.right - RESIZE_EDGE_THREHDLE &&
                   mouse_y <= window_rect.bottom && mouse_y > window_rect.bottom - RESIZE_EDGE_THREHDLE) {
            result = HTBOTTOMRIGHT;
        }
    }

    return result;
}


bool QuickFramelessView::event(QEvent *event) {
    if (event->type() == QEvent::Type::WindowStateChange) {
        if (this->windowState() == Qt::WindowState::WindowMaximized)
            this->adjustResizeContentMargins(true);
        else
            this->adjustResizeContentMargins(false);
    }
    return QQuickWindow::event(event);
}

void QuickFramelessView::adjustResizeContentMargins(bool isMaximized) {
    RECT frame = {0, 0, 0, 0};
    ::AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
    frame.left = std::abs(frame.left);
    frame.top = std::abs(frame.bottom);
    qreal pixel_ratio = this->devicePixelRatio();
    QQuickItem *content_item = this->contentItem();
    QQuickItem *mainlayout_item = content_item->childItems().at(0)->childItems().at(1);
    if (mainlayout_item) {
        if (isMaximized) {
            QMetaObject::invokeMethod(mainlayout_item, "adjustResizeConentMargins",
                                      Q_ARG(QVariant, static_cast<int>(frame.left / pixel_ratio)),
                                      Q_ARG(QVariant, static_cast<int>(frame.top / pixel_ratio)),
                                      Q_ARG(QVariant, static_cast<int>(frame.right / pixel_ratio)),
                                      Q_ARG(QVariant, static_cast<int>(frame.bottom / pixel_ratio)));
        } else {
            QMetaObject::invokeMethod(mainlayout_item, "adjustResizeConentMargins",
                                      Q_ARG(QVariant, 0),
                                      Q_ARG(QVariant, 0),
                                      Q_ARG(QVariant, 0),
                                      Q_ARG(QVariant, 0));
        }
    }
}

void QuickFramelessView::setTitleBar(QQuickItem *title_bar) {

    if (!title_bar)
        return;
    this->m_titleBar = title_bar;
}

int QuickFramelessView::dealWindowTitleBar(const int &x, const int &y) {
    //support high dpi 标题栏
    int result = 0;
    if (!this->m_titleBar)
        return 0;
    qreal dpr = this->devicePixelRatio();
    QPointF pos = this->m_titleBar->mapFromGlobal(QPointF(x / dpr, y / dpr));
    if (this->m_titleBar->contains(pos)) {
        QQuickItem *child = this->m_titleBar->childAt(pos.x(), pos.y());
        if (!child) {
            result = HTCAPTION;
        } else {
            if (this->whitelistItem.contains(child))
                result = HTCAPTION;
        }
    }

    return result;
}

void QuickFramelessView::addTitleWhitelist(QQuickItem *item) {
    if (!item)
        return;
    if (!this->whitelistItem.contains(item))
        this->whitelistItem.append(item);
}
