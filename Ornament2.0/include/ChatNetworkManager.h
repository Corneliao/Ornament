#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include"../global.h"
class ChatNetworkManager : public QObject
{
	Q_OBJECT
		enum MSGTYPE { SignIn = 0, NormalMessage, CronyApplication, updateCronyList };
public:
	ChatNetworkManager(QObject* parent = Q_NULLPTR);
	~ChatNetworkManager();
public:
	void initializeSocket();
private:
	QTcpSocket* socket = Q_NULLPTR;
signals:
	void connecterrorSignal();
	void connectedSignal();
};
