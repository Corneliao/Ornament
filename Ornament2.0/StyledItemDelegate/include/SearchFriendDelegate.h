#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include "../../global.h"
class SearchFriendDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	SearchFriendDelegate(QObject* parent);
	~SearchFriendDelegate();
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const override;
};
