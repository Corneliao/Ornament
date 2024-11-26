#include "../include/SystemNotification.h"

SystemNotification::SystemNotification(QWidget* parent)
	: QWidget(parent)
{
	this->text = new QLabel("测试", this);
	this->text->adjustSize();

	this->setFixedSize(this->text->width() + 20, this->text->height() + 10);
	this->text->setGeometry(QRect(QPoint(this->rect().center().x() - (this->text->width() / 2), this->rect().center().y() - (this->text->height() / 2)), QSize(this->text->size())));

	this->shadow = new QGraphicsDropShadowEffect(this);
	this->shadow->setBlurRadius(10);
	this->shadow->setOffset(0, 0);
	this->shadow->setColor(Qt::gray);
	this->setGraphicsEffect(this->shadow);
}

SystemNotification::~SystemNotification()
{
}

void SystemNotification::setText(const QString& _text)
{
	this->text->setText(_text);
	this->text->adjustSize();
	this->setFixedSize(this->text->width() + 20, this->text->height() + 10);
	this->text->setGeometry(QRect(QPoint(this->rect().center().x() - (this->text->width() / 2), this->rect().center().y() - (this->text->height() / 2)), QSize(this->text->size())));
}

void SystemNotification::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRoundedRect(this->rect(), 10, 10);
}
