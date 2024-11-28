#include "../include/SystemNotificationDelegate.h"

SystemNotificationDelegate::SystemNotificationDelegate(QWidget* parent)
	: QStyledItemDelegate(parent)
{
}

SystemNotificationDelegate::~SystemNotificationDelegate()
{
}

void SystemNotificationDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (!index.isValid())
		return;
	NoticeType type = index.data(Qt::UserRole + 1).value<NoticeType>();
	if (type == NoticeType::UserApplication) {
	}
}

QSize SystemNotificationDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize();
}
