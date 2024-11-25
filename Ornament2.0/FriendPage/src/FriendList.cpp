#include "../include/FriendList.h"

FriendList::FriendList(QWidget* parent)
	: QWidget(parent)
{
	this->setFixedWidth(250);

	QVBoxLayout* main_vbox = new QVBoxLayout(this);
	this->setLayout(main_vbox);

	this->friend_listView = new QListView(this);
	main_vbox->addWidget(friend_listView);
}

FriendList::~FriendList()
{
}