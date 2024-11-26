#pragma once

#include <QPixmap>
#include <QString>

extern qreal GLOB_ScaleDpi;
extern bool GLOB_IsConnectedMysql;
extern bool GLOB_IsConnectedServer;
extern int GLOB_UserAccount;
extern QString GLOB_UserName;
extern QSize screenSize;
//extern QPixmap GLOB_UserHead;

struct FeatureBarData {
	QPixmap normal_ico;
	QPixmap select_ico;
	QString ico_text;
};

struct UserData {
	QPixmap userHead;
	QString userName;
	int userAccount;
	bool status = false;
};

struct FriendChatData {
	QPixmap userHead;
	QString userName;
	QString userMessage;
};
struct SearchFriendData {
	QByteArray imagebytes;
	QString userName;
	bool status;
	QString userAccount;
};
