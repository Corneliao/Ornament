#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPainter>
#include <QStackedLayout>
#include "FriendChatList.h"
#include "DefaultWindow.h"
#include "ChatWindow.h"
class ChatPage : public QWidget
{
	Q_OBJECT

public:
	ChatPage(QWidget* parent);
	~ChatPage();
public:
	void CreateChatWindow(const QListWidgetItem* item);
	void setCurrentChatWindow(const QListWidgetItem* item);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	FriendChatList* friendChat_list = Q_NULLPTR;
	DefaultWindow* default_window = Q_NULLPTR;
	QStackedLayout* stack_layout = Q_NULLPTR;
signals:
};
