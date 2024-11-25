#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include "FriendChatList.h"
#include "DefaultWindow.h"

class ChatPage : public QWidget
{
	Q_OBJECT

public:
	ChatPage(QWidget* parent);
	~ChatPage();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	FriendChatList* friend_list = Q_NULLPTR;
	DefaultWindow* default_window = Q_NULLPTR;
};
