#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QListWidget>
#include <QGraphicsDropShadowEffect>
#include "NormalTitleBar.h"
class SystemNotification : public QWidget
{
	Q_OBJECT

public:
	SystemNotification(QWidget* parent);
	~SystemNotification();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QListWidget* notificationItemList = Q_NULLPTR;
	QGraphicsDropShadowEffect* shadow = Q_NULLPTR;
	NormalTitleBar* title_bar = Q_NULLPTR;
};
