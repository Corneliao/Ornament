#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include"../../global.h"
class SystemNotificationDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	SystemNotificationDelegate(QWidget* parent);
	~SystemNotificationDelegate();
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const Q_DECL_OVERRIDE;
};
