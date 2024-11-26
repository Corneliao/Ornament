#pragma once

#include <QWidget>
#include <QPropertyAnimation>
#include <QEvent>
#include <QKeyEvent>
#include <QListWidget>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QLineEdit>
#include <QTimeLine>
#include "../Component/include/NormalTitleBar.h"
#include "../FramelessWindow/include/framelesswindow.h"
#include "../Component/include/LineEdit.h"
#include "../StyledItemDelegate/include/SearchFriendItemWidget.h"
#include "../Component/include/SystemNotification.h"

class AddFriend : public FranelessWindowForDialog
{
	Q_OBJECT

public:
	AddFriend(QDialog* parent = Q_NULLPTR);
	~AddFriend();
	void increaseSearchMember(const SearchFriendData& data);
	void addFriendButtonClicked();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
	void showEvent(QShowEvent*)Q_DECL_OVERRIDE;
	void searchFriend(const QString& userAccount);
	void mousePressEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
private:
	NormalTitleBar* titleBar = Q_NULLPTR;
	QPropertyAnimation* sizeAnimation = Q_NULLPTR;
	LineEdit* search = Q_NULLPTR;
	QListWidget* listWidget = Q_NULLPTR;
	QGraphicsDropShadowEffect* shadow = Q_NULLPTR;
	QTimeLine* windowSizeAnimation = Q_NULLPTR;
	bool isDraging = false;
	QPoint startPoint;
	QPoint m_offset;
	SystemNotification* notification = Q_NULLPTR;
	QPropertyAnimation* noticeAnimation = Q_NULLPTR;
signals:
	void closeWindow();
	void searchFriendSignal(const  QString& userAccount);
	void sendFriendApplication(const QString& receiverAccount);
private:
};
