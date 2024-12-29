#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QLabel>
#include "../../global.h"
class FriendListDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	FriendListDelegate(QObject* parent);
	~FriendListDelegate();
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const override;
};
