#pragma once

#include <QObject>
#include <QThread>
#include "TcpServer.h"
#include "ClientWork.h"
#include "FileTcpServer.h"
#include "FileWork.h"
class OrnamentServer :public QObject
{
	Q_OBJECT

public:
	OrnamentServer(QObject* parent = Q_NULLPTR);
	~OrnamentServer();
private:
	void newClientConnection(const qintptr handle);
	void newFileServiceConnection(const qintptr handle);
	void dealNewClientLogin(const  QString& userAccount);
	void dealClientDisconnecioned(int socket_id);
	void dealClientApplication(const QString& sender, const QString& receiver);
	void dealNoticeSenderUserUpdateFriendListSignal(const QString& senderAccount);
	void dealAcceptUserNormalMesasge(const QString& senderUserAccount, const QString& receiverUserAccount, const QString& message);
	void dealNoticeDisconnected(const QString& userAccount);
	void dealFileTasks();
	//---FILE---
	void dealServiceDisconnection(const QString& socket_id);
	void dealSendFileForServer(const QString& senderAccount, const QString& receiveAccount, const QString& filePath);
private:
	TcpServer* tcpServer = Q_NULLPTR;
	FileTcpServer* file_tcpServer = Q_NULLPTR;
	QMap<QThread*, ClientWork*>  clients;
	QMap<QThread*, FileWork* > fileServices;
signals:
	void transferNewClientLogin(const QString& userAccount);
	void transferClientApplication(const QString& sender, const QString& receiver);
	void transferNoticeSenderUserUpdateFriendListSignal(const QString& senderAccount);
	void transferAcceptUserNormalMessage(const QString& senderUserAccount, const QString& receiverUserAccount, const QString& message);
	void transferNoticeDisconnected(const QString& userAccount);
	void transferSendFileForServer(const QString& senderAccount, const QString& receiveAccount, const QString& filePath);
};
