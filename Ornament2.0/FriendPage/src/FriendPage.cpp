#include "../include/FriendPage.h"

FriendPage::FriendPage(QWidget* parent)
	: QWidget(parent)
{
	qRegisterMetaType<QList<FriendListData>>("QList<FriendListData>");
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->friend_list = new FriendList(this);
	main_lay->addWidget(this->friend_list);
	main_lay->addStretch();

	connect(this, &FriendPage::userFriendList, this->friend_list, &FriendList::initializeFriends, Qt::DirectConnection);
}

FriendPage::~FriendPage()
{
}

void FriendPage::updateFriendCurrentStatus(const QString& cronyAccount)
{
	this->friend_list->updateFriendCurrentStatus(cronyAccount);
}
