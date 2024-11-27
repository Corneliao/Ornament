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
	void  increaseFriendItem(const QString& userHead, const QString& username);

protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QListWidget* chat_listWidget = Q_NULLPTR;
};
