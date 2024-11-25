#include "../include/FriendPage.h"

FriendPage::FriendPage(QWidget* parent)
	: QWidget(parent)
{
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->friend_list = new FriendList(this);
	main_lay->addWidget(this->friend_list);

	main_lay->addStretch();
}

FriendPage::~FriendPage()
{
}