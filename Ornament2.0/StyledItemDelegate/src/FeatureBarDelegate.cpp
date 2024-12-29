#include "../include/FeatureBarDelegate.h"

FeatureBarDelegate::FeatureBarDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

FeatureBarDelegate::~FeatureBarDelegate()
{
}

void FeatureBarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;
	FeatureBarData item_data = index.data(Qt::UserRole).value<FeatureBarData>();
	QRect item_rect = option.rect;
	item_rect.adjust(2, 2, -2, -2);

	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing, true);
	//绘制图标
	QRect ico_rect(item_rect.left() + 10, item_rect.center().y() - (25 / 2), 25, 25);

	//改变选中状态

	if (option.state.testFlag(QStyle::State_Selected) || option.state.testFlag(QStyle::State_MouseOver)) {
		painter->save();
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor(135, 206, 250, 50));
		painter->drawRoundedRect(item_rect, 10, 10);
		painter->restore();

		painter->save();
		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::NoBrush);
		painter->drawPixmap(ico_rect, item_data.select_ico.scaled(QSize(20 * GLOB_ScaleDpi, 20 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter->restore();

		//绘制文本
		painter->save();
		painter->setPen(QColor::fromString("#2775fe"));
		painter->setBrush(Qt::NoBrush);
		painter->drawText(item_rect, Qt::AlignCenter, item_data.ico_text);
		painter->restore();

		//绘制边线

		painter->save();
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor::fromString("#2775fe"));
		painter->drawRoundedRect(QRect(item_rect.right() - 10, item_rect.center().y() - ((item_rect.height() - 15) / 2), 3, item_rect.height() - 15), static_cast<qreal>(3) / 2, static_cast<qreal>(3) / 2);
		painter->restore();
	}
	else {
		painter->save();
		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::NoBrush);
		painter->drawPixmap(ico_rect, item_data.normal_ico.scaled(QSize(20 * GLOB_ScaleDpi, 20 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter->restore();

		//绘制文本
		painter->save();
		painter->setPen(QColor::fromString("#757f98"));
		painter->setBrush(Qt::NoBrush);
		painter->drawText(item_rect, Qt::AlignCenter, item_data.ico_text);
		painter->restore();
	}

	//QStyledItemDelegate::paint(painter, option, index);
}

QSize FeatureBarDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(40);
	return size;
}