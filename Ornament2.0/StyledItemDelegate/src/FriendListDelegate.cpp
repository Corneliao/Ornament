#include "../include/FriendListDelegate.h"

FriendListDelegate::FriendListDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

FriendListDelegate::~FriendListDelegate()
{
}

void FriendListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;

	QStyleOptionViewItem viewoption(option);
	initStyleOption(&viewoption, index);

	UserData user_data = index.data(Qt::UserRole).value<UserData>();
	//FriendListData friend_data = index.data(Qt::UserRole).value<FriendListData>();
	QRect item_rect = option.rect;
	item_rect.adjust(2, 2, -2, -2);

	painter->setRenderHints(QPainter::Antialiasing);
	painter->setRenderHints(QPainter::TextAntialiasing);
	painter->setRenderHints(QPainter::SmoothPixmapTransform);

	painter->save();
	if (option.state.testFlag(QStyle::State_MouseOver) || option.state.testFlag(QStyle::State_Selected)) {
		painter->setPen(QPen(QColor::fromString("#a5b3cb"), 1));
		painter->setBrush(QColor::fromString("#f1f3f9"));
		painter->drawRoundedRect(item_rect, 10, 10);
	}
	else {
		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::transparent);
		painter->drawRoundedRect(item_rect, 10, 10);
	}
	painter->restore();

	//绘制头像
	QRect userheadRect = QRect(item_rect.left() + 5, item_rect.center().y() - ((item_rect.height() - 15) / 2), item_rect.height() - 15, item_rect.height() - 15);
	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::NoBrush);
	painter->drawPixmap(userheadRect, user_data.userHead.scaled(QSize(userheadRect.size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	painter->restore();

	QLabel temp(user_data.userName);
	temp.adjustSize();

	//昵称
	QRect  userNameRect = QRect(userheadRect.right() + 8, item_rect.center().y() - (temp.height() / 2), temp.width(), temp.height());
	painter->save();
	painter->setPen(Qt::black);
	painter->setBrush(Qt::NoBrush);
	painter->drawText(userNameRect, Qt::AlignCenter, user_data.userName);
	painter->restore();

	temp.setText(user_data.status_text);
	temp.adjustSize();

	QRect status_text_rect(item_rect.right() - (temp.width() + 10), item_rect.center().y() - (temp.height() / 2), temp.width(), temp.height());

	//在线图标
	QRect status_ico_rect = QRect(status_text_rect.left() - (8 + 10), item_rect.center().y() - (8 / 2), 8, 8);
	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::NoBrush);
	painter->drawPixmap(status_ico_rect, user_data.status_ico.scaled(QSize(status_ico_rect.size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	painter->restore();

	painter->save();
	painter->setPen(Qt::black);
	painter->setBrush(Qt::NoBrush);
	painter->drawText(status_text_rect, Qt::AlignCenter, user_data.status_text);
	painter->restore();
}

QSize FriendListDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(50);
	return size;
}
