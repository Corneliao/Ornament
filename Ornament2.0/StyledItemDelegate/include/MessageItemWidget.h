#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include "../../global.h"
class Bubble;
class ProgressSlider;
class MessageItemWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MessageItemWidget(const UserData& userdata, QWidget* parent);
	~MessageItemWidget();
	UserData currentMessageItemData()const;
	void setSliderPosition(const qreal& position);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;

private:
	QLabel* userHead = Q_NULLPTR;
	QLabel* userName = Q_NULLPTR;
	UserData user_data;
	Bubble* bubble = Q_NULLPTR;
};
class Bubble :public QWidget {
public:
	explicit Bubble(const UserData& user_data, QWidget* parent = Q_NULLPTR);
	void setSliderPosition(const qreal& position);
	void setStatus(const QString& text);
protected:
	void paintEvent(QPaintEvent* event)Q_DECL_OVERRIDE;
	void showEvent(QShowEvent* event)Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
private:
	QLabel* text = Q_NULLPTR;
	QLabel* fileIco = Q_NULLPTR;
	QLabel* fileName = Q_NULLPTR;
	QLabel* fileSize = Q_NULLPTR;
	ProgressSlider* slider = Q_NULLPTR;
	QLabel* status = Q_NULLPTR;
	QRect border_rect;
};

class ProgressSlider :public QWidget {
public:
	explicit ProgressSlider(QWidget* parent = Q_NULLPTR);
	void setSliderPosition(const qreal& position);

protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
	void showEvent(QShowEvent*)Q_DECL_OVERRIDE;
private:
	qreal  m_position = 0.0;
};
