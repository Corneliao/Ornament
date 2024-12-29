#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QRandomGenerator>
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QMutex>
#include  "UserDatabaseManager.h"
#include "FileWork.h"
#include "FileTcpServer.h"
class ClientWork : public QObject
{
	Q_OBJECT
		enum MSGTYPE {
		Login = 0, NoticeNewLogin, FriendApplication, WaitAcceptApplication, SendAcceptApplicationNotice, AcceptedApplication,
		NormalMessage, SendNormalMesssage, SendUserDisconnected
	};
public:
	ClientWork(const qintptr& handle, QObject* parent = Q_NULLPTR);
	~ClientWork();
public:
	void initializeSocket();
	int socketId() const;
	void noticeUserLogin(const QString& useraccount, bool isExist);  //通知用户有好友登录
	void accpetClientApplication(const QString& sender, const QString& receiver);
	void sendAddNewUserSucceedNotice(const QString& senderAccount);
	Q_INVOKABLE void senderUserMessageToReceiver(const QString& senderUserAccount, const QString& message);
	void dealNoticeClientDisconnected(const QString& user_account);
	void NoticeClientDisconnectedToUser(const QString& userAccount, bool isExist);
	QString currentUserAccount()const;
private:
	void ReadData();
	void dealClientDisonnected();
private:
	QThread* sql_thread = Q_NULLPTR;
	UserDatabaseManager* userDatabase = Q_NULLPTR;
	QTcpSocket* socket = Q_NULLPTR;
	qintptr socketDecriptor = 0;
	int socket_id = 0;
	QString userAccount;
	QString receiverUserAccountTemp;
	QMutex mutex;

signals:
	void clientDisconnected(int socket_id);
	void updateClientStatus(const QString& userAccount, const QString& type); //接受到用户登录触发此信号 更新用户在线状态
	void newClientLogin(const QString& userAccount); //用户登录触发信号
	void NoticeClientLoginSignal(const QString& userAccount); //让每次线程去数据库中 查询是否有此好友  返回结果
	void AcceptClientApplicationSignal(const QString& sender, const QString& receiver); //有用户发送好友申请触发此信号
	void NoticeSenderUserUpdateFriendListSignal(const QString& senderAccount); //接受方已同意好友申请，通知发送方更新好友列表
	void acceptUserNorlmalMessage(const QString& senderUserAccount, const QString& reiceiverUserAccount, const QString& message); //接受好友发送的文本消息
	void NoticeClientDisconnected(const QString& userAccount); //通知用户 好友离线
	void FindFreindForDisconnected(const QString& userAccount); //用户离线时查询数据库是否有此好友4
};
