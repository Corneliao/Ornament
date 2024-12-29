#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QLabel>
#include "../../global.h"
class FeatureBarDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	FeatureBarDelegate(QObject* parent);
	~FeatureBarDelegate();
protected:
	void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const override;
};
