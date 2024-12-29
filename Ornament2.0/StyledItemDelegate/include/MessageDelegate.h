#pragma once

#include <QStyledItemDelegate>
#include <QLabel>
#include <QPainter>
#include  "../../global.h"
class MessageDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	MessageDelegate(QObject* parent);
	~MessageDelegate();
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const override;
};
