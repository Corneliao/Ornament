#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include "../../StyledItemDelegate/include/FriendChatDelegate.h"
#include "../../Component/include/RoundImage.h"
class FriendChatList : public QWidget
{
	Q_OBJECT

public:
	FriendChatList(QWidget* parent);
	~FriendChatList();
public:
	void  increaseFriendItem(UserData& user_data);
	bool isExistFriendChatItem(const QString& account);
	void setItemSelected(const QString& account);

protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QListWidget* chat_listWidget = Q_NULLPTR;
signals:
	void FriendChatItemChanged(const QListWidgetItem* item);
};
