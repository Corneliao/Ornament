#include "../include/FriendPage.h"

FriendPage::FriendPage(QWidget* parent)
	: QWidget(parent)
{
	qRegisterMetaType<QList<UserData>>("QList<UserData>");
	QHBoxLayout* main_lay = new QHBoxLayout(this);
	this->setLayout(main_lay);

	this->friend_list = new FriendList(this);
	main_lay->addWidget(this->friend_list);
	main_lay->addStretch();

	connect(this, &FriendPage::userFriendList, this->friend_list, &FriendList::initializeFriends, Qt::DirectConnection);
	connect(this->friend_list, &FriendList::createChatWindowSignal, this, &FriendPage::createChatWindowSignal, Qt::DirectConnection);
	connect(this->friend_list, &FriendList::ItemChanged, this, &FriendPage::itemChanged,Qt::DirectConnection);
}

FriendPage::~FriendPage()
{
}

void FriendPage::updateFriendCurrentStatus(const QString& cronyAccount)
{
	this->friend_list->updateFriendCurrentStatus(cronyAccount);
}

void FriendPage::IncreaseNewUserItem(const UserData& user_data)
{
	this->friend_list->increaseUserFriendItem(user_data);
}

UserData FriendPage::getUserData(const QString& userAccount) const
{
	UserData data = this->friend_list->getUserData(userAccount);
	return data;
}

void FriendPage::setUserDataForDisconnected(const QString& userAccount)
{
	this->friend_list->setUserDataForDisconnected(userAccount);
}
