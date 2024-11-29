#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QListWidget>
#include <QGraphicsDropShadowEffect>
#include "../../StyledItemDelegate/include/UserApplicationItemWidget.h"
#include "NormalTitleBar.h"
#include "../../global.h"
class SystemNotification : public QWidget
{
	Q_OBJECT

public:
	SystemNotification(QWidget* parent);
	~SystemNotification();
public:
	void IncreaseUserApplicationItem(const UserData& user_data);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
	void removeApplicationItem();
private:
	QGraphicsDropShadowEffect* shadow = Q_NULLPTR;
	NormalTitleBar* title_bar = Q_NULLPTR;
	QLabel* userApplicationNotification = Q_NULLPTR;
	QListWidget* userApplicationNotificationItemList = Q_NULLPTR;
	QLabel* system_Notification = Q_NULLPTR;
	QListWidget* systemNotificationItemList = Q_NULLPTR;
signals:
	void agreeApplicationSignal(const QString& userAccount);
	void disAgreeApplicationSignal();
	void NotificationEmptySignal();
};
