#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "FriendInfo.h"
#include "FriendList.h"
#include "../../global.h"
class FriendPage : public QWidget
{
	Q_OBJECT

public:
	FriendPage(QWidget* parent);
	~FriendPage();
	void updateFriendCurrentStatus(const QString& cronyAccount);
private:
	FriendInfo* friend_info = Q_NULLPTR;
	FriendList* friend_list = Q_NULLPTR;
signals:
	void userFriendList(const QList<FriendListData> datas);
};
