#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include"../global.h"
class ChatNetworkManager : public QObject
{
	Q_OBJECT
		enum MSGTYPE { Login = 0, NormalMessage, CronyApplication, updateCronyList, NoticeNewLogin};
public:
	ChatNetworkManager(QObject* parent = Q_NULLPTR);
	~ChatNetworkManager();
public:
	void initializeSocket();
	void sendLoginInfo();
	void sendApplication(const QString& receiver);
	void ReadData();
private:
	QTcpSocket* socket = Q_NULLPTR;
signals:
	void connecterrorSignal();
	void connectedSignal();
	void UserLogined(const QString  cronyAccount);
};
