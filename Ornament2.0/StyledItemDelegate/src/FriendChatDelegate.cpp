#include "../include/FriendChatDelegate.h"

FriendChatDelegate::FriendChatDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

FriendChatDelegate::~FriendChatDelegate()
{
}

void FriendChatDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;

	FriendChatData chat_data = index.data(Qt::UserRole).value<FriendChatData>();

	if (chat_data.userHead.isNull() || chat_data.userName.isEmpty())
		return;

	QStyleOptionViewItem viewoption(option);
	initStyleOption(&viewoption, index);

	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
	QRect item_rect = option.rect;
	item_rect.adjust(2, 2, -2, -2);

	//绘制头像
	QRect userheadRect = QRect(item_rect.left() + 5, item_rect.center().y() - ((item_rect.height() - 15) / 2), item_rect.height() - 15, item_rect.height() - 15);

	QLabel temp(chat_data.userName);
	temp.adjustSize();
	QRect userNameRect = QRect(userheadRect.right() + 7, userheadRect.top() + 5, temp.width(), temp.height());

	temp.setText(chat_data.userMessage);
	temp.adjustSize();
	QRect userMessageRect = QRect(userNameRect.left(), userNameRect.bottom() + 5, temp.width(), temp.height());

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

	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::NoBrush);
	painter->drawPixmap(userheadRect, chat_data.userHead.scaled(QSize(userheadRect.size() * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	painter->restore();

	painter->save();
	painter->setPen(Qt::black);
	painter->setBrush(Qt::NoBrush);
	painter->drawText(userNameRect, Qt::AlignCenter, chat_data.userName);
	painter->restore();

	painter->save();
	painter->setPen(Qt::gray);
	painter->setBrush(Qt::NoBrush);
	painter->drawText(userMessageRect, Qt::AlignLeft, chat_data.userMessage);
	painter->restore();
}

QSize FriendChatDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(60);
	return size;
}
