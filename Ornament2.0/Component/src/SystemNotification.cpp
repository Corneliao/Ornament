#include "../include/SystemNotification.h"

SystemNotification::SystemNotification(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(250, 200);
	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	main_vbox->setContentsMargins(0, 0, 0, 0);
	this->setLayout(main_vbox);

	this->title_bar = new NormalTitleBar("通知", this);
	this->title_bar->setCloseVisible(true);
	this->notificationItemList = new QListWidget(this);
	main_vbox->addWidget(this->title_bar);
	main_vbox->addWidget(this->notificationItemList);

	this->shadow = new QGraphicsDropShadowEffect(this);
	this->shadow->setOffset(0, 0);
	this->shadow->setBlurRadius(8);
	this->shadow->setColor(Qt::gray);
	this->setGraphicsEffect(this->shadow);
}

SystemNotification::~SystemNotification()
{
}

void SystemNotification::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
	painter.drawRoundedRect(this->rect(), 10, 10);
}