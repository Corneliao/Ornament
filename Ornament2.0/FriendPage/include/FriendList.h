#pragma once

#include <QWidget>
#include <QListView>
#include <QVBoxLayout>
#include <QPainter>
#include <QListWidget>
#include <qsystemtrayicon.h>
#include "../../Component/include/RoundImage.h"
#include "../../StyledItemDelegate/include/FriendListDelegate.h"
#include "../../global.h"
#include "../../Component/include/SystemTrayIconNotification.h"
class FriendList : public QWidget
{
	Q_OBJECT

public:
	FriendList(QWidget* parent);
	~FriendList();
public:
	void initializeFriends(const QList<UserData> datas);
	void increaseUserFriendItem(const UserData& friend_data);
	void updateFriendCurrentStatus(const QString& cronyAccount);
	UserData getUserData(const QString& userAccount)const;
	void dealItemClicked(QListWidgetItem* item);
	void dealItemChanged(QListWidgetItem* item);
	void setUserDataForDisconnected(const  QString &  userAccount);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QListWidget* friend_list = Q_NULLPTR;
	QLabel* title = Q_NULLPTR;
signals:
	void createChatWindowSignal(UserData& user_data);
	void ItemChanged(UserData& user_data);
};
