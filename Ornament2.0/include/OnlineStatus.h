#pragma once

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include "../Component/include/RoundImage.h"
class OnlineStatus : public QWidget
{
	Q_OBJECT

public:
	OnlineStatus(QWidget* parent);
	~OnlineStatus();
	void setOnlineStatus(bool isOnline);
protected:
	void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
private:
	QLabel* ico = Q_NULLPTR;
	QRect drawPenRect;
	QLabel* text = Q_NULLPTR;
	bool isOnline = false;
};
