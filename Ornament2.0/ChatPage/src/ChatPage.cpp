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

void ChatPage::CreateChatWindow(UserData& user_data)
{
	//判断是否已存在此聊天窗口
	int isExist = this->friendChat_list->isExistFriendChatItem(user_data.userAccount);
	if (isExist != -1) {
		this->friendChat_list->setItemData(isExist, user_data);
		ChatWindow* chat_window;
		for (int i = 1; i < this->stack_layout->count(); i++) {
			chat_window = qobject_cast<ChatWindow*>(this->stack_layout->widget(i));
			if (chat_window) {
				if (chat_window->currentUserData().userAccount == user_data.userAccount) {
					chat_window->IncreaseMessageItem(user_data);
					return;
				}
			}
		}
	}
	user_data.index = this->stack_layout->count();
	ChatWindow* chat_window = new ChatWindow(user_data, this);
	QListWidgetItem* item = new QListWidgetItem();
	item->setData(Qt::UserRole, QVariant::fromValue(user_data));
	this->ChatItemAndChatWindow.insert(item, chat_window);
	this->friendChat_list->increaseFriendItem(item);
	this->stack_layout->addWidget(chat_window);
	for (int i = 1; i < this->stack_layout->count(); i++) {
		chat_window = qobject_cast<ChatWindow*>(this->stack_layout->widget(i));
		if (chat_window) {
			if (chat_window->currentUserData().userAccount == user_data.userAccount) {
				chat_window->IncreaseMessageItem(user_data);
				break;
			}
		}
	}
	connect(chat_window, &ChatWindow::SendUserMessage, this, &ChatPage::SendUserMessage, Qt::DirectConnection);
}

void ChatPage::DoubleClickCreateChatWindow(UserData& user_data)
{
	int isExist = this->friendChat_list->isExistFriendChatItem(user_data.userAccount);
	if (isExist != -1)
		return;
	user_data.index = this->stack_layout->count();
	ChatWindow* chat_window = new ChatWindow(user_data, this);
	QListWidgetItem* item = new QListWidgetItem();
	item->setData(Qt::UserRole, QVariant::fromValue(user_data));
	this->ChatItemAndChatWindow.insert(item, chat_window);
	this->stack_layout->addWidget(chat_window);
	this->stack_layout->setCurrentWidget(chat_window);
	this->friendChat_list->increaseFriendItem(item);
	this->friendChat_list->setItemSelected(user_data.userAccount);
	connect(chat_window, &ChatWindow::SendUserMessage, this, &ChatPage::SendUserMessage, Qt::DirectConnection);
}

void ChatPage::setCurrentChatWindow(const UserData& user_data)
{
	this->stack_layout->setCurrentIndex(user_data.index);
}

/**
 * @brief 当好友上线时 更新item和聊天窗口的数据
 * @param user_data
 */
void ChatPage::itemChanged(const UserData& user_data)
{
	UserData temp;
	for (QListWidgetItem* item : this->ChatItemAndChatWindow.keys()) {
		temp = item->data(Qt::UserRole).value<UserData>();
		if (temp.userAccount == user_data.userAccount) {
			item->setData(Qt::UserRole, QVariant::fromValue(user_data));
			this->ChatItemAndChatWindow.value(item)->setChatWindowData(user_data);
			return;
		}
	}
}

void ChatPage::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::transparent);
	painter.drawRoundedRect(this->rect(), 15, 15);
}