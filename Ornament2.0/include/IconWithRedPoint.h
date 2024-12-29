#pragma once

#include <QWidget>
#include <QLabel>
#include <QPainter>
class IconWithRedPoint : public QWidget
{
	Q_OBJECT

public:
	IconWithRedPoint(const QSize &size,const QPixmap & pixmap = QPixmap(),QWidget *parent = Q_NULLPTR);
	~IconWithRedPoint();
	void setIcon(const QPixmap& pixmap);
	void isUnread(bool unread);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLabel* ico = Q_NULLPTR;
	QPixmap ico_pixmap;
	bool m_isUnread = false;
};
