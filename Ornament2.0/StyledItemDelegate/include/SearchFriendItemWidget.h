#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include "../../Component/include/RoundImage.h"
#include "../../global.h"
class SearchAddFriendButton;
class SearchFriendItemWidget : public QWidget
{
	Q_OBJECT

public:
	SearchFriendItemWidget(const SearchFriendData& friend_data, QWidget* parent);
	~SearchFriendItemWidget();
protected:
	bool eventFilter(QObject* target, QEvent* event) Q_DECL_OVERRIDE;
private:
	QLabel* userHead = Q_NULLPTR;
	QLabel* userName = Q_NULLPTR;
	QLabel* statusIco = Q_NULLPTR;
	QLabel* statusText = Q_NULLPTR;
	SearchAddFriendButton* button = Q_NULLPTR;
	SearchFriendData m_friend_data;
signals:
	void SendFreindApplication(const QString& receiverAccount);
};
class SearchAddFriendButton :public QWidget
{
public:
	explicit SearchAddFriendButton(QWidget* parent = Q_NULLPTR);
protected:
	void paintEvent(QPaintEvent*)Q_DECL_OVERRIDE;
private:
	QLabel* text = Q_NULLPTR;
};
