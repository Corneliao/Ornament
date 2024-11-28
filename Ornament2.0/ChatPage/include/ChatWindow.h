#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include "../../global.h"
class ChatTitle;
class ChatWindow : public QWidget
{
	Q_OBJECT

public:
	explicit ChatWindow(const UserData& user_data, QWidget* parent);
	~ChatWindow();
	UserData currentUserData()const;
private:
	UserData m_userData;
	ChatTitle* chat_title = Q_NULLPTR;
	int currentChatWindowIndex = 0;
};

class ChatTitle :public QWidget {
public:
	explicit ChatTitle(const QString& user_name, const QPixmap& user_head, QWidget* parent);
	~ChatTitle();
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLabel* userHead = Q_NULLPTR;
	QLabel* userName = Q_NULLPTR;
	QGraphicsDropShadowEffect* shadow = Q_NULLPTR;
};
