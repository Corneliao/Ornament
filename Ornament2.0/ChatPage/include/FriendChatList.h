﻿#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include "../../StyledItemDelegate/include/FriendChatDelegate.h"
#include "../../Component/include/RoundImage.h"

QT_BEGIN_NAMESPACE
class ListWidgetItem;
QT_END_NAMESPACE

class FriendChatList : public QWidget
{
	Q_OBJECT

public:
	FriendChatList(QWidget* parent);
	~FriendChatList();
public:
	void increaseFriendItem(QListWidgetItem* item);
	int isExistFriendChatItem(const QString& account);
	void setItemSelected(const QString& account);
	void setItemData(int index, const UserData& user_data);
	void dealItemClicked(QListWidgetItem* item);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QListWidget* chat_listWidget = Q_NULLPTR;
	QLabel* title = Q_NULLPTR;
	QLabel* edit = Q_NULLPTR;
signals:
	void FriendChatItemChanged(const UserData& user_ata);
};

class ListWidgetItem :public QListWidgetItem {
public:
	explicit ListWidgetItem();
	~ListWidgetItem();
protected:
	bool operator<(const QListWidgetItem& other) const override;
};
