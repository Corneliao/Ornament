#pragma once

#include <QtWidgets/QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QThread>
#include <QEvent>
#include <QStackedLayout>
#include <QResizeEvent>
#include "../FramelessWindow/include/framelesswindow.h"
#include "../UserDataManager/include/UserDatabaseManager.h"
#include "ApplicationTitleBar.h"
#include "TitleTool.h"
#include "ApplicationFeaureBar.h"
#include "../ChatPage/include/ChatPage.h"
#include "ChatNetworkManager.h"
#include "../FriendPage/include/FriendPage.h"
class Ornament : public FramelessWindow
{
	Q_OBJECT

public:
	Ornament(const QPixmap& userhead_pixmap, const QByteArray& imagebytes, const QString& userName, const  int& userAccount, QWidget* parent = nullptr);
	~Ornament();
	void startSqlThread();
protected:
	void resizeEvent(QResizeEvent* event)Q_DECL_OVERRIDE;
private:
	void showTool();
	void maxWindowSlot();
	void changeEvent(QEvent* event)Q_DECL_OVERRIDE;
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
signals:
	void ToolStateSignal(bool isShow);
};
