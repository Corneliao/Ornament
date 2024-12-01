#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include"../global.h"
class ChatNetworkManager : public QObject
{
	Q_OBJECT
		enum MSGTYPE { Login = 0, NoticeNewLogin, FriendApplication, WaitAcceptApplication, SendAcceptApplicationNotice, AcceptedApplication, NormalMessage, SendNormalMessage };
public:
	ChatNetworkManager(QObject* parent = Q_NULLPTR);
	~ChatNetworkManager();
public:
	void initializeSocket();
	void sendLoginInfo();
	void sendApplication(const QString& receiver);
	void ReadData();
	void acceptApplication(const QString& userAccount);
	void sendUserNormalMessage(const QString& senderUserAccount, const QString& receiverUserAccount, const QString& message);
private:
	QTcpSocket* socket = Q_NULLPTR;
signals:
	void connecterrorSignal();
	void connectedSignal();
	void UserLogined(const QString  cronyAccount);
	void updateUserFriendList(const QString& cronyAccount, int type);  //成功添加好友获取用户数据信息
	void acceptUserApplication(const QString& userAccount, int DataType);
	void acceptUserNormalMessage(const QString& senderUserAccount, const QString& message);
};
