#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QPainter>
#include "../global.h"
#include "../Component/include/RoundImage.h"
#include "IconWithRedPoint.h"
#include "OnlineStatus.h"
#include "AddFriend.h"
class ApplicationTitleBar : public QWidget
{
	Q_OBJECT

public:
	ApplicationTitleBar(QWidget* parent);
	~ApplicationTitleBar();
	void setUserHead(const QByteArray& imagebytes);
	void setUnfoldIcon(bool isShow);
	void setOnlineStatus(bool isOnline);
protected:
	QPixmap setIconDpi(const QSize& size, const QPixmap& pixmap);
	bool eventFilter(QObject* target, QEvent* event)Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLabel* window_ico = Q_NULLPTR;
	QLabel* window_text = Q_NULLPTR;
	qreal dpi = 0.0;
	QLabel* user_head = Q_NULLPTR;
	QPixmap user_head_pixmap;
	QLabel* unfold = Q_NULLPTR;
	QLabel* search = Q_NULLPTR;
	QLabel* notification = Q_NULLPTR;
	OnlineStatus* online_status = Q_NULLPTR;
	AddFriend* add_friend = Q_NULLPTR;
signals:
	void showToolSignal();
	void showAddFriendSignal();
	void showSystemNotification();
};
