#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include "../../global.h"
#include <QLabel>
class FriendChatDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	FriendChatDelegate(QObject *parent);
	~FriendChatDelegate();
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
