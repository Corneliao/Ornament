#pragma once

#include <QWidget>
#include <QListView>
#include <QVBoxLayout>
#include <QPainter>
#include <QListWidget>
#include <QStandardItemModel>
#include "../../Component/include/RoundImage.h"
#include "../../StyledItemDelegate/include/FriendListDelegate.h"
#include "../../global.h"
class FriendList : public QWidget
{
	Q_OBJECT

public:
	FriendList(QWidget* parent);
	~FriendList();
public:
	void initializeFriends(const QList<FriendListData> datas);
	void increaseUserFriendItem(const FriendListData friend_data);
	void updateFriendCurrentStatus(const QString& cronyAccount);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QListView* friend_listView = Q_NULLPTR;
	QStandardItemModel* model = Q_NULLPTR;
};
