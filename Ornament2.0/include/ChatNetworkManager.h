#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QFile>
#include"../global.h"
class ChatNetworkManager : public QObject
{
	Q_OBJECT
		enum MSGTYPE {
		Login = 0, NoticeNewLogin, FriendApplication, WaitAcceptApplication, SendAcceptApplicationNotice, AcceptedApplication, NormalMessage, SendNormalMessage, SendUserDisconnected
		, SendFileInfo, ReceivedFileInfo, SendFileData
	};
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
	void SendUserFile(const QString& senderUserAccount, const QString& receiverUserAccount, const FileInfoData& file_data);
	void SendFileDataToServer();
private:
	QTcpSocket* socket = Q_NULLPTR;
	QString filePath;
	qint64 fileTotalSize = 0;
signals:
	void connecterrorSignal();
	void connectedSignal();
	void UserLogined(const QString  cronyAccount);
	void updateUserFriendList(const QString& cronyAccount, int type);  //成功添加好友获取用户数据信息
	void acceptUserApplication(const QString& userAccount, int DataType);
	void acceptUserNormalMessage(const QString& senderUserAccount, const QString& message);
	void userDisconnectedSignal(const QString& userAccount);
	void ReceiveFileForServertSignal(const QString senderAccount, const QString fileName, const qint64 fileSize);
	void updateUploadFileProgress(const qreal& pos);
};
