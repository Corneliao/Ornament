#include "../include/MessageDelegate.h"

MessageDelegate::MessageDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

MessageDelegate::~MessageDelegate()
{
}

void MessageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;
	UserData user_data = index.data(Qt::UserRole).value<UserData>();
	QRect item_rect = option.rect;

	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);

	QLabel temp(user_data.userName);
	temp.adjustSize();

	QRect userhead_rect;
	QRect username_rect;
	QRect bubble_rect;
	if (user_data.alignment == Qt::AlignLeft) {
		//头像
		userhead_rect = QRect(item_rect.left() + 3, item_rect.top() + 3, 40, 40);
		//昵称
		username_rect = QRect(QPoint(userhead_rect.right() + 5, userhead_rect.top()), QSize(temp.size()));

		//绘制气泡
		temp.setText(user_data.userMessage);
		temp.adjustSize();
		bubble_rect = QRect(username_rect.left(), username_rect.bottom() + 5, temp.width() + 20, temp.height() + 10);
	}
	else if (user_data.alignment == Qt::AlignRight) {
		//头像
		userhead_rect = QRect(item_rect.right() - (40), item_rect.top() + 3, 40, 40);
		//昵称
		username_rect = QRect(QPoint(userhead_rect.left() - (temp.width() + 5), userhead_rect.top()), QSize(temp.size()));

		//绘制气泡
		temp.setText(user_data.userMessage);
		temp.adjustSize();
		bubble_rect = QRect(username_rect.right() - (temp.width() + 20), username_rect.bottom() + 5, temp.width() + 20, temp.height() + 10);
	}

	//绘制头像

	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::NoBrush);
	painter->drawPixmap(userhead_rect, user_data.userHead.scaled(QSize(40 * GLOB_ScaleDpi, 40 * GLOB_ScaleDpi), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	painter->restore();

	//绘制昵称
	QFont font;
	font.setPixelSize(10);
	font.setBold(true);
	painter->save();
	painter->setPen(Qt::black);
	painter->setFont(font);
	painter->setBrush(Qt::NoBrush);
	painter->drawText(username_rect, Qt::AlignCenter, user_data.userName);
	painter->restore();

	QRect message_rect;
	message_rect.setSize(temp.size());
	message_rect.moveCenter(bubble_rect.center());

	painter->save();
	if (user_data.alignment == Qt::AlignLeft) {
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor::fromString("#6a4cff"));
		painter->drawRoundedRect(bubble_rect, 10, 10);
	}
	else {
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor::fromString("#0099ff"));
		painter->drawRoundedRect(bubble_rect, 10, 10);
	}
	painter->restore();

	painter->save();
	painter->setPen(Qt::white);
	painter->setBrush(Qt::NoBrush);
	painter->drawText(message_rect, Qt::AlignLeft, user_data.userMessage);
	painter->restore();
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	UserData user_data = index.data(Qt::UserRole).value<UserData>();
	QLabel temp(user_data.userMessage);
	temp.adjustSize();

	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(40 + temp.height());
	return size;
}
