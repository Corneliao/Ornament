#include "../include/FriendChatList.h"

FriendChatList::FriendChatList(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(200);

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);

	QPalette pale;
	pale.setColor(QPalette::Window, Qt::transparent);
	this->chat_listWidget = new QListWidget(this);
	this->chat_listWidget->setPalette(pale);
	this->chat_listWidget->setFrameShape(QFrame::NoFrame);

	FriendChatDelegate* delegate = new  FriendChatDelegate(this);
	this->chat_listWidget->setItemDelegate(delegate);

	main_vbox->addWidget(this->chat_listWidget);

	connect(this->chat_listWidget, &QListWidget::itemChanged, this, &FriendChatList::FriendChatItemChanged, Qt::DirectConnection);
}

FriendChatList::~FriendChatList()
{
}

void FriendChatList::increaseFriendItem(UserData& user_data)
{
	user_data.userMessage = "...";
	user_data.index = this->chat_listWidget->count() + 1;
	QListWidgetItem* item = new  QListWidgetItem(this->chat_listWidget);
	item->setData(Qt::UserRole, QVariant::fromValue(user_data));
	this->chat_listWidget->addItem(item);
	item->setSelected(true);
}

bool FriendChatList::isExistFriendChatItem(const QString& account)
{
	for (int i = 0; i < this->chat_listWidget->count(); i++) {
		QListWidgetItem* item = this->chat_listWidget->item(i);
		UserData user_data = item->data(Qt::UserRole).value<UserData>();
		if (user_data.userAccount == account) {
			return true;
		}
	}
	return false;
}

void FriendChatList::setItemSelected(const QString& account)
{
	for (int i = 0; i < this->chat_listWidget->count(); i++) {
		QListWidgetItem* item = this->chat_listWidget->item(i);
		UserData user_data = item->data(Qt::UserRole).value<UserData>();
		if (user_data.userAccount == account) {
			item->setSelected(true);
			return;
		}
	}
	;
}

void FriendChatList::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(220, 220, 220, 55));
	painter.drawRoundedRect(this->rect(), 15, 15);
}