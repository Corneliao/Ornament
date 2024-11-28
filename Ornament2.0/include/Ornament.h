#pragma once

#include <QtWidgets/QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QThread>
#include <QEvent>
#include <QStackedLayout>
#include <QResizeEvent>
#include <QApplication>
#include <QTimeLine>
#include "../Component/include/SystemNotification.h"
#include "../FramelessWindow/include/framelesswindow.h"
#include "../UserDataManager/include/UserDatabaseManager.h"
#include "ApplicationTitleBar.h"
#include "TitleTool.h"
#include "ApplicationFeaureBar.h"
#include "../ChatPage/include/ChatPage.h"
#include "ChatNetworkManager.h"
#include "../FriendPage/include/FriendPage.h"
#include "AddFriend.h"
class Ornament : public FramelessWindow
{
	Q_OBJECT

public:
	Ornament(const QPixmap& userhead_pixmap, const QByteArray& imagebytes, const QString& userName, const  int& userAccount, QWidget* parent = nullptr);
	~Ornament();
	void startSqlThread();
	void deleteChatThread();
protected:
	void resizeEvent(QResizeEvent* event)Q_DECL_OVERRIDE;
	bool eventFilter(QObject* target, QEvent* event)Q_DECL_OVERRIDE;
	void showEvent(QShowEvent*)Q_DECL_OVERRIDE;
	void changeEvent(QEvent* event)Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event)Q_DECL_OVERRIDE;
private:
	void showTool();
	void showAddFriend();
	void showSystemNotification();
	void SystemNotificationAnimationFrameChanged(int frame);
	void maxWindowSlot();
private:
	ApplicationTitleBar* application_title_Bar = Q_NULLPTR;
	ApplicationFeaureBar* application_feature_Bar = Q_NULLPTR;
	UserDatabaseManager* userDataBase = Q_NULLPTR;
	QThread* sql_thread = Q_NULLPTR;
	TitleTool* tool = Q_NULLPTR;
	bool isShowMax = false;
	QStackedLayout* stack_layout = Q_NULLPTR;
	ChatPage* chat_page = Q_NULLPTR;
	QThread* chat_thread = Q_NULLPTR;
	ChatNetworkManager* chat_network_manager = Q_NULLPTR;
	FriendPage* friend_page = Q_NULLPTR;
	AddFriend* addFriend = Q_NULLPTR;
	SystemNotification* systemNotification = Q_NULLPTR;
	QTimeLine* systemNotification_Animation = Q_NULLPTR;
signals:
	void ToolStateSignal(bool isShow);
	void searchFriendSignal(const QString& userAccount);
	void SearchFriendDataSignal(const SearchFriendData& data);
	void SendFriendApplication(const QString& receiverAccount);
	void isSendApplication(bool isSucceed);
	void existTheUserSignal();
	//void createChatWindowSignal();
};
