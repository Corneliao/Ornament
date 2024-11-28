#pragma once

#include <QWidget>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
class Notification : public QWidget
{
	Q_OBJECT

public:
	Notification(QWidget* parent);
	~Notification();
public:
	void setText(const QString& _text);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QGraphicsDropShadowEffect* shadow = Q_NULLPTR;
	QLabel* text = Q_NULLPTR;
};
