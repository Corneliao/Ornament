#include "../include/ChatPage.h"

ChatPage::ChatPage(QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->friend_list = new FriendChatList(this);
	this->default_window = new DefaultWindow(this);

	main_lay->addWidget(this->friend_list);
	main_lay->addWidget(this->default_window);
//	main_lay->addStretch();
}

ChatPage::~ChatPage()
{
}



void ChatPage::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::transparent);
	painter.drawRoundedRect(this->rect(), 15, 15);
}