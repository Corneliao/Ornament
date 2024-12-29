#include "../include/SearchFriendDelegate.h"

SearchFriendDelegate::SearchFriendDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{
}

SearchFriendDelegate::~SearchFriendDelegate()
{
}

void SearchFriendDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;
	SearchFriendData friend_data = index.data(Qt::UserRole).value<SearchFriendData>();
}

QSize SearchFriendDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(50);
	return size;
}