#include "../include/IconWithRedPoint.h"

IconWithRedPoint::IconWithRedPoint(const QSize& size, const QPixmap& pixmap, QWidget* parent)
	: QWidget(parent)
{
	this->setFixedSize(size);
	this->ico = new QLabel(this);
	this->ico->setFixedSize(18, 18);
	this->ico->setScaledContents(true);
	this->ico->setPixmap(pixmap);
	this->ico->setGeometry(QRect(QPoint(this->rect().center().x() - (this->ico->width() / 2), this->rect().center().y() - (this->ico->height() / 2)), QSize(this->ico->size())));
}

IconWithRedPoint::~IconWithRedPoint()
{
}

void IconWithRedPoint::setIcon(const QPixmap& pixmap)
{
	this->ico->setPixmap(pixmap);
}

void IconWithRedPoint::isUnread(bool unread)
{
	this->m_isUnread = unread;
	this->update();
}
void IconWithRedPoint::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	if (this->m_isUnread) {
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::red);
		painter.drawEllipse(QRect(QPoint(this->rect().right() - 10, this->rect().top() + 1), QSize(6, 6)));
	}
}
