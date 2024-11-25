#pragma once

#include <QWidget>
#include <QListView>
#include <QVBoxLayout>
class FriendList : public QWidget
{
	Q_OBJECT

public:
	FriendList(QWidget* parent);
	~FriendList();
private:
	QListView* friend_listView = Q_NULLPTR;
};
