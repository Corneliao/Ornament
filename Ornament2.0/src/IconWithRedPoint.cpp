#include "../include/IconWithRedPoint.h"

IconWithRedPoint::IconWithRedPoint(const QSize& size, const QPixmap& pixmap, QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(size);

	this->ico = new QLabel(this);
	this->ico->setFixedSize(size.width() - 10, size.height() - 10);
	this->ico->setScaledContents(true);
	//QPixmap pixmap();
	this->ico->setPixmap(pixmap);
}

IconWithRedPoint::~IconWithRedPoint()
{
}

void IconWithRedPoint::setIcon(const QPixmap& pixmap)
{
	this->ico->setPixmap(pixmap);
}

void IconWithRedPoint::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::red);
	painter.drawEllipse(QRect(QPoint(this->rect().right() - 10, this->rect().bottom() - 10), QSize(10, 10)));
}
