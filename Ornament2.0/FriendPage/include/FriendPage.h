#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "FriendInfo.h"
#include "FriendList.h"
class FriendPage : public QWidget
{
	Q_OBJECT

public:
	FriendPage(QWidget* parent);
	~FriendPage();
private:
	FriendInfo* friend_info = Q_NULLPTR;
	FriendList* friend_list = Q_NULLPTR;
};
