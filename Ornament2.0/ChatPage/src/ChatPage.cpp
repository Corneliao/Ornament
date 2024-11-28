#include "../include/ChatPage.h"

ChatPage::ChatPage(QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->friendChat_list = new FriendChatList(this);
	this->default_window = new DefaultWindow(this);

	this->stack_layout = new QStackedLayout;
	this->stack_layout->addWidget(this->default_window);
	main_lay->addWidget(this->friendChat_list);
	main_lay->addLayout(this->stack_layout);
	connect(this->friendChat_list, &FriendChatList::FriendChatItemChanged, this, &ChatPage::setCurrentChatWindow, Qt::DirectConnection);
}

ChatPage::~ChatPage()
{
}

void ChatPage::CreateChatWindow(const QListWidgetItem* item)
{
	//判断是否已存在此聊天窗口
	UserData user_data = item->data(Qt::UserRole).value<UserData>();
	bool isExist = this->friendChat_list->isExistFriendChatItem(user_data.userAccount);
	if (isExist)
		return;
	user_data.index = this->stack_layout->count();
	ChatWindow* chat_window = new ChatWindow(user_data, this);
	this->stack_layout->addWidget(chat_window);
	//this->stack_layout->setCurrentWidget(chat_window);
	this->friendChat_list->increaseFriendItem(user_data);
	this->friendChat_list->setItemSelected(user_data.userAccount);
}

void ChatPage::setCurrentChatWindow(const QListWidgetItem* item)
{
	UserData user_data = item->data(Qt::UserRole).value<UserData>();
	this->stack_layout->setCurrentIndex(user_data.index);
}

void ChatPage::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::transparent);
	painter.drawRoundedRect(this->rect(), 15, 15);
}